/*******************************************************************************
 * func.c
 *员工管理系统(epoll)
 * @fcopyright  Copyright 2021 SIC Microelectronics CO. Ltd.
 * @fauthor     [JiangYing] [2405596758@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2021-03-08] Create file.
 ******************************************************************************/
#include "head.h"
sqlite3* db=NULL;
char* errmsg=NULL;
int sfd;
char sql[256]="";


//启动服务器
int ServerInit(){
	//socket
	int res;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0){
		perror("\033[31m>>socket error\033[0m");
		return -1;
	}
	//允许端口快速重用
	int reuse=1;
	res=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	if(res<0){
		perror("\033[31m>>setsocketopt error\033[0m");
		return -1;	
	}
	//bind:绑定服务器IP和端口
	struct sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(PORT);
	sin.sin_addr.s_addr=inet_addr(IP);

	res=bind(sfd,(struct sockaddr*)&sin,sizeof(sin));
	if(res<0){
		perror("\033[31m>>bind error\033[0m");
		return -1;		
	}
	//listen:监听
	res=listen(sfd,4);
	if(res<0){
		perror("\033[31m>>listen error\033[0m");
		return -1;			
	}
	//等待客户端连接
	struct sockaddr_in cin;
	socklen_t len=sizeof(cin);
	int port=0;
	char ip[20]="";
	int nfds=0;

	//创建epoll
	int epfd;
	epfd=epoll_create(MAX_USER_COUNT);
	if(epfd<0){	
		perror("\033[31m>>epoll_create error\033[0m");
		return -1;			
	}
	struct clinfo info;
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=sfd;
	res=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);
	if(epfd<0){	
		perror("\033[31m>>epoll_ctl error\033[0m");
		return -1;			
	}
	info.epfd=epfd;
	info.fd=sfd;
	//启动数据库
	printf("\033[32m 服务器启动成功...\n \033[0m");	
	if(SqliteInit()<0){
		printf("\033[31m数据库启动失败...\n \033[0m");	
	}
	else{
		printf("\033[32m数据库启动成功...\n \033[0m");		
	}

	//循环
	while(1){
		EpollMain(&info,sfd);
	}
	//close
	close(epfd);
	close(sfd);
	return 0;
}
//服务器epoll主函数
int EpollMain(cinfo* info,int sfd){
	int i;
	int nfds;//保存准备好的文件描述符的数量
	int fd;//文件描述符

	nfds=epoll_wait(info->epfd,info->events,EVENT_COUNT,8);//8ms超市

	for(i=0;i<nfds;i++){
		if(!(info->events[i].events & EPOLLIN)) continue;//不是可读event 	
		if(info->events[i].data.fd==sfd){
			//有新的连接
			NewConnect(info);
		}
		else{
			//旧的连接读事件
			Read(info,i);
		}
	}
}
//旧的连接事件
void Read(cinfo* info,int i){
	struct message msg;
	bzero(msg.buf,sizeof(msg.buf));
	int confd=info->events[i].data.fd;
	int res;
	res=read(confd,&msg,sizeof(Msg));
	if(res==0){
		printf("\033[33m>>断开连接...\n\033[0m");
		epoll_ctl(info->epfd,EPOLL_CTL_DEL,confd,NULL);//删除连接的服务器
		close(confd);
	}
	else{
		//解析消息协议
		printf(">>code=%s\n",msg.code);
		AnalyMsg(&msg,confd);
	}
}
//新的连接事件
void NewConnect(cinfo* info){
	//新的连接事件将他加入到事件表
	struct sockaddr_in cin;
	int len=sizeof(cin);
	int confd;
	char ip[20]="";
	int port;
	confd=accept(info->fd,(struct sockaddr*)&cin,&len);
	inet_ntop(AF_INET,&cin.sin_addr,ip,sizeof(ip));
	port=ntohs(cin.sin_port);
	printf("\033[33m>>[%s:%d]连接成功...\n\033[0m",ip,port);
	struct epoll_event ev;
	ev.events=EPOLLIN | EPOLLET;
	ev.data.fd=confd;
	epoll_ctl(info->epfd,EPOLL_CTL_ADD,confd,&ev);
}
//打开数据库
int SqliteInit(){
	int res;
	char* errmsg=NULL;
	res=sqlite3_open("./mysq.db",&db);
	if(res!=0){
		printf("\033[31m>>数据库打开失败%s...\n\033[0m",sqlite3_errmsg(db));
		return -1;
	}
	bzero(sql,sizeof(sql));
	sprintf(sql,"create table if not exists User(name char,passwd char,Status char)");
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0){
		printf("\033[31m>>创建表user失败:%s...\n\033[0m",errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	sprintf(sql,"create table if not exists Staff(Name char,Address char,\
				Age int,Phone char,Salary float,Part char,Level int,Number char,ID char)");
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0){
		printf("\033[31m>>创建表Staff失败:%s...\n\033[0m",errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from User");
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=0){
		printf("\033[31m>>获取表失败%d:%s...\n\033[0m",__LINE__,errmsg);
		return -1;
	}
}
//关闭数据库
void SqliteClose(){
	int res;
	res=sqlite3_close(db);
	if(res!=0){
		printf("\033[31m>>数据库关闭失败...\n\033[0m");	
		return;
	}
	printf("\033[32m>>数据库关闭...\n\033[0m");	
}
//解析消息协议
void AnalyMsg(Msg* msg,int cfd){
	if(strcmp(msg->code,"AdminSignIn")==0){
		//管理员登录事件
		AdminSignIn(msg,cfd);
	}
	if(strcmp(msg->code,"UserSignIn")==0){
		//用户登录事件
		UserSignIn(msg,cfd);
	}
	if(strcmp(msg->code,"Register")==0){
		//注册事件
		SignUp(msg,cfd);
	}
	if(strcmp(msg->code,"Add")==0){
		//添加用户事件
		AddUser(msg,cfd);
	}
	if(strcmp(msg->code,"DelUsrID")==0){
		//删除用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"delete from Staff where ID=\"%s\"",msg->umsg.ID);
		DelUser(msg,cfd);
	}
	if(strcmp(msg->code,"DelUsrPhone")==0){
		//删除用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"delete from Staff where Phone=\"%s\"",msg->umsg.phone);
		DelUser(msg,cfd);
	}
	if(strcmp(msg->code,"FindUsrPhone")==0){
		//查询用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"select *  from Staff where Phone=\"%s\"",msg->umsg.phone);
		FindUser(msg,cfd);
	}
	if(strcmp(msg->code,"FindUsrName")==0){
		//查询用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"select *  from Staff where Name=\"%s\"",msg->umsg.name);
		FindUser(msg,cfd);
	}
	if(strcmp(msg->code,"FindUsrID")==0){
		//查询用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"select *  from Staff where ID=\"%s\"",msg->umsg.ID);
		FindUser(msg,cfd);
	}
	if(strcmp(msg->code,"FindUsrNumber")==0){
		//查询用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"select *  from Staff where Number=\"%s\"",msg->umsg.number);
		FindUser(msg,cfd);
	}
	if(strcmp(msg->code,"UpdatePhone")==0){
		//修改用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"UPDATE Staff set Name=\"%s\",Address=\"%s\",Age=%d,\
				Phone=\"%s\" ,Salary=%f ,Part=\"%s\" ,Level=%d ,Number=\"%s\" ,ID=\"%s\"\
				where Phone=\"%s\"",msg->umsg.name,msg->umsg.addr,msg->umsg.age,\
				msg->umsg.phone,msg->umsg.salary,msg->umsg.part,msg->umsg.level,msg->umsg.number,\
				msg->umsg.ID,msg->umsg.phone);
		UpdateUser(msg,cfd);
	}
	if(strcmp(msg->code,"UpdateID")==0){
		//修改用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"UPDATE Staff set Name=\"%s\" ,Address=\"%s\" ,Age=%d,\
				Phone=\"%s\" ,Salary=%f ,Part=\"%s\", Level=%d ,Number=\"%s\" ,ID=\"%s\"\
				where ID=\"%s\"",msg->umsg.name,msg->umsg.addr,msg->umsg.age,\
				msg->umsg.phone,msg->umsg.salary,msg->umsg.part,msg->umsg.level,msg->umsg.number,\
				msg->umsg.ID,msg->umsg.ID);
		UpdateUser(msg,cfd);
	}
	if(strcmp(msg->code,"UpdateNumber")==0){
		//修改用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"UPDATE Staff set Name=\"%s\" ,Address=\"%s\" ,Age=%d,\
				Phone=\"%s\" ,Salary=%f ,Part=\"%s\" ,Level=%d ,Number=\"%s\" ,ID=\"%s\"\
				where Number=\"%s\"",msg->umsg.name,msg->umsg.addr,msg->umsg.age,\
				msg->umsg.phone,msg->umsg.salary,msg->umsg.part,msg->umsg.level,msg->umsg.number,\
				msg->umsg.ID,msg->umsg.number);
		UpdateUser(msg,cfd);
	}
	if(strcmp(msg->code,"UserUpdate")==0){
		//查询用户事件	
		bzero(sql,sizeof(sql));
		sprintf(sql,"UPDATE Staff set Name=\"%s\" ,Address=\"%s\" ,Age=%d,\
				Phone=\"%s\" ,Salary=%f ,Part=\"%s\" ,Level=%d ,Number=\"%s\" ,ID=\"%s\"\
				where Number=\"%s\"",msg->umsg.name,msg->umsg.addr,msg->umsg.age,\
				msg->umsg.phone,msg->umsg.salary,msg->umsg.part,msg->umsg.level,msg->umsg.number,\
				msg->umsg.ID,msg->umsg.name);
		UpdateUser(msg,cfd);
	}	
}
//管理员登录事件
void AdminSignIn(Msg *msg,int cfd){
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]请求管理员登录...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//查询数据库的用户名是否存在
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from User where name=\"%s\"",msg->user);
	printf(">>sql=%s\n",sql);
	char **result;
	int row;
	int col;
	char* errmsg=NULL;
	res=sqlite3_get_table(db,sql,&result,&row,&col,&errmsg);
	printf("res=%d\n",res);
	if(res!=SQLITE_OK){
		printf("\033[31m获取数据库表项失败:%s...\n\033[0m",errmsg);
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"获取数据库表失败\n");
		Send(&cmsg,cfd);
		return;
	}
	if(row>0){
		//查询到相关用户
		//比对密码，看能否登录
		int  index=col;
		int i;
		for(i=0;i<row;i++)
		{
			if(strcmp(result[index],msg->user)==0){
				if(strcmp(result[index+1],msg->passwd)==0){
					if(strcmp(result[index+2],"Admin")==0){
						//匹配成功，发送消息给客户端
						printf("\033[31m登陆成功...\n\033[0m");
						strcpy(cmsg.code,"AdminConnect");
						strcpy(cmsg.buf,"管理员登陆成功\n");
						Send(&cmsg,cfd);
						return;
					}
					else{
						//不是管理员
						printf("\033[31m用户不是管理员:...\033[0m");
						strcpy(cmsg.code,"Error");
						strcpy(cmsg.buf,"账号不是管理员\n");
						Send(&cmsg,cfd);
					}
				}
				else{
					//密码不正确
	printf(">>%d>>%d\n",__LINE__,row);
					printf("\033[31m管理员密码错误:...\033[0m");
					strcpy(cmsg.code,"Error");
					strcpy(cmsg.buf,"管理员密码错误\n");
					Send(&cmsg,cfd);
				}
			}
			index+=col;
		}
	}
	else{
		//没有查询到相关记录
		//向客户端发送未查询到该用户消息
		printf("\033[31m没有查询到%s:%s...\n\033[0m",msg->user,errmsg);
		strcpy(cmsg.code,"None");
		strcpy(cmsg.buf,"没有查询到该用户\n");
		Send(&cmsg,cfd);
		return;
	}
	sqlite3_free_table(result);
}
//向客户端发送消息
void Send(Msg* msg,int cfd){
	int res;                                          
	do{
		res=send(cfd,msg,sizeof(Msg),0);
	}while(res<0 && errno == EINTR);

	if(res<0){
		perror("\033[31m>>无法连接客户端...\033[0m");
		return;
	}
}
//注册事件
void SignUp(Msg* msg,int cfd){
	printf("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]注册请求...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//比较用户名是否被注册过
	int row;
	int col;
	char** result;
	char* errmsg=NULL;
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from User where name=\"%s\"",msg->user);
	res=sqlite3_get_table(db,sql,&result,&row,&col,&errmsg);
	printf(">>%d:%d\n",__LINE__,res);
	if(res==0){
		if(row>0){
			//存在该用户无法注册
			printf("\033[31m用户存在无法注册:%s...\n\033[0m",sqlite3_errmsg(db));
			//向客户端发送报错信息；
			strcpy(cmsg.code,"Error");
			strcpy(cmsg.buf,"该用户已经被注册\n");
			Send(&cmsg,cfd);
			return;
		}
	}
	else{
		printf("\033[31m插入数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"注册管理员失败\n");
		Send(&cmsg,cfd);
		return;	
	}
	sqlite3_free_table(result);
	printf(">>%d\n",__LINE__);
	//注册该用户
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into User values(\"%s\",\"%s\",\"%s\")",msg->user,msg->passwd,msg->Status);
	printf(">>sql=%s\n",sql);
	res=sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	printf("res=%d\n",res);
	if(res!=SQLITE_OK){
		printf("\033[31m插入数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"注册管理员失败\n");
		Send(&cmsg,cfd);
		return;
	}
	else{
		//向客户端发送成功插入到该用户消息
		strcpy(cmsg.code,"OK");
		strcpy(cmsg.buf,"注册管理员成功\n");
		Send(&cmsg,cfd);
		return;	
	}
}
//管理员登录事件函数的回调函数
int AdminSignInHandle(void* argi,int f_num, char** f_value,char** f_name){

}
//添加用户
void AddUser(Msg* msg,int cfd){
	printf("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]添加用户请求...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//比较用户名是否被注册过
	int row;
	int col;
	char** result;
	char* errmsg=NULL;
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from Staff where Number=\"%s\"",msg->umsg.number);
	res=sqlite3_get_table(db,sql,&result,&row,&col,&errmsg);
	printf(">>%d:%d\n",__LINE__,res);
	if(res==0){
		if(row>0){
			//存在该用户无法注册
			printf("\033[31m员工存在无法注册:%s...\n\033[0m",sqlite3_errmsg(db));
			//向客户端发送报错信息；
			strcpy(cmsg.code,"Error");
			strcpy(cmsg.buf,"该员工已经被注册\n");
			Send(&cmsg,cfd);
			return;
		}
	}
	else{
		printf("\033[31m插入数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"添加新用户失败\n");
		Send(&cmsg,cfd);
		return;	
	}
	sqlite3_free_table(result);
	printf(">>%d\n",__LINE__);
	//注册该用户
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into Staff values(\"%s\",\"%s\",%d,\"%s\",%f,\"%s\",%d,\"%s\",\"%s\")"\
			,msg->umsg.name,msg->umsg.addr,msg->umsg.age,msg->umsg.phone,msg->umsg.salary,msg->umsg.part,\
			msg->umsg.level,msg->umsg.number,msg->umsg.ID);
	printf(">>sql=%s\n",sql);
	res=sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	printf("res=%d\n",res);
	if(res!=SQLITE_OK){
		printf("\033[31m插入数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"添加用户失败\n");
		Send(&cmsg,cfd);
		return;
	}
	else{
		//向客户端发送成功插入到该用户消息
		strcpy(cmsg.code,"OK");
		strcpy(cmsg.buf,"添加用户成功\n");
		Send(&cmsg,cfd);
		return;	
	}

}
//删除用户
void DelUser(Msg* msg,int cfd){
	printf("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]删除用户请求...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//删除该用户
	printf(">>sql=%s\n",sql);
	res=sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	printf("res=%d\n",res);
	if(res!=SQLITE_OK){
		printf("\033[31m删除数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"删除用户失败\n");
		Send(&cmsg,cfd);
		return;
	}
	else{
		//向客户端发送成功插入到该用户消息
		strcpy(cmsg.code,"OK");
		strcpy(cmsg.buf,"删除用户成功\n");
		Send(&cmsg,cfd);
		return;	
	}


}
//查询用户
void FindUser(Msg* msg,int cfd){
	printf("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]查询用户请求...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//查询该用户
	char **result;
	int row;
	int col;
	int i,j;
	printf(">>sql=%s\n",sql);
	res=sqlite3_get_table(db,sql,&result,&row,&col,&errmsg);
	printf("res=%d;row=%d;col=%d\n",res,row,col);
	if(res!=SQLITE_OK){
		printf("\033[31m查询数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"查询用户失败\n");
		Send(&cmsg,cfd);
		return;
	}
	else{
		//向客户端发送成功插入到该用户消息
		printf("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
		strcpy(cmsg.code,"FindInfo");
		cmsg.port=row;
		strcpy(cmsg.buf,"Name\tAddr\tAge\tPhone\tSalary\tPart\tLevel\tNumber\tID\n");
		Send(&cmsg,cfd);
		
		for(i=1;i<row+1;i++){
			strcpy(cmsg.code,"OK");
			bzero(cmsg.buf,sizeof(cmsg.buf));
			sprintf(cmsg.buf,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",\
					result[i*col+0],result[i*col+1],result[i*col+2],\
					result[i*col+3],result[i*col+4],result[i*col+5],\
					result[i*col+6],result[i*col+7],result[i*col+8]);
			Send(&cmsg,cfd);
		}
		
		/*	
		for(i=0;i<row;i++){
			for(j=0;j<col;j++){
				strcpy(cmsg.code,"OK");
				sprintf(cmsg.buf,"%s\t",result[index]);
				Send(&cmsg,cfd);
				index++;
			}
			strcpy(cmsg.code,"OK");
			strcpy(cmsg.buf,"\n");
			Send(&cmsg,cfd);
		}
		*/
	}	
}
//更新用户
void UpdateUser(Msg* msg,int cfd){
	printf("[%s:%s:%d]\n",__FILE__,__func__,__LINE__);
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]修改用户请求...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//修改该用户
	printf(">>sql=%s\n",sql);
	res=sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	printf("res=%d\n",res);
	if(res!=SQLITE_OK){
		printf("\033[31m插入数据库表项失败:%s...\n\033[0m",sqlite3_errmsg(db));
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"修改用户失败\n");
		Send(&cmsg,cfd);
		return;
	}
	else{
		//向客户端发送成功插入到该用户消息
		strcpy(cmsg.code,"OK");
		strcpy(cmsg.buf,"修改用户成功\n");
		Send(&cmsg,cfd);
		return;	
	}
}
//用户登录事件
void UserSignIn(Msg* msg,int cfd){
	int res=0;
	struct message cmsg;//发往客户端的消息
	bzero(cmsg.buf,sizeof(cmsg.buf));
	printf("\033[33m>>[%s]用户请求登录...\n\033[0m",msg->ip);
	printf("\033[33m>>[%s:%s:%s]...\n\033[0m",msg->user,msg->passwd,msg->Status);	
	//查询数据库的用户名是否存在
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from User where name=\"%s\"",msg->user);
	printf(">>sql=%s\n",sql);
	char **result;
	int row;
	int col;
	char* errmsg=NULL;
	res=sqlite3_get_table(db,sql,&result,&row,&col,&errmsg);
	printf("res=%d\n",res);
	if(res!=SQLITE_OK){
		printf("\033[31m获取数据库表项失败:%s...\n\033[0m",errmsg);
		//向客户端发送报错信息；
		strcpy(cmsg.code,"Error");
		strcpy(cmsg.buf,"获取数据库表失败\n");
		Send(&cmsg,cfd);
		return;
	}
	if(row>0){
		//查询到相关用户
		//比对密码，看能否登录
		int  index=col;
		int i;
		for(i=0;i<row;i++)
		{
			if(strcmp(result[index],msg->user)==0){
				if(strcmp(result[index+1],msg->passwd)==0){
					//匹配成功，发送消息给客户端
					printf("\033[31m登陆成功...\n\033[0m");
					strcpy(cmsg.code,"OK");
					strcpy(cmsg.buf,"用户登陆成功\n");
					Send(&cmsg,cfd);
					return;
				}
				else{
					//密码不正确
					printf(">>%d>>%d\n",__LINE__,row);
					printf("\033[31m用户密码错误:...\033[0m");
					strcpy(cmsg.code,"Error");
					strcpy(cmsg.buf,"用户密码错误\n");
					Send(&cmsg,cfd);
				}
			}
			index+=col;
		}
	}
	else{
		//没有查询到相关记录
		//向客户端发送未查询到该用户消息
		printf("\033[31m没有查询到%s:%s...\n\033[0m",msg->user,errmsg);
		strcpy(cmsg.code,"None");
		strcpy(cmsg.buf,"没有查询到该用户\n");
		Send(&cmsg,cfd);
		return;
	}
	sqlite3_free_table(result);
}
