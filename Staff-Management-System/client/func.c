/*******************************************************************************
 * func.c
 *员工管理系统
 * @fcopyright  Copyright 2021 SIC Microelectronics CO. Ltd.
 * @fauthor     [JiangYing] [2405596758@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2021-03-08] Create file.
 ******************************************************************************/
#include "head.h"

int x=10;
int y=0;
int cfd;
char Status[20]="";


//主界面
void MainMenu(){
	int choice;
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>登录\033[0m               \n");
		printf("/*    \033[31m 2>>注册\033[0m               \n");
		printf("/*    \033[31m 0>>退出\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		getchar();
		printf("\033[0m");
		switch(choice){
			case 0:
				//退出界面
				printf("\033[0m\033[1m >>程序退出...\033[0m\n");
				goto END;
			case 1:
				//登录界面
				SignInMenu();
				break;
			case 2:
				//注册界面
				Register();
				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
		printf("\033[0m");
		return;
}
//登录界面
void SignInMenu(){
	int choice;
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>管理员登录\033[0m         \n");
		printf("/*    \033[31m 2>>用户登录\033[0m           \n");
		printf("/*    \033[31m 0>>返回上级菜单\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 0:
				//返回上级菜单
				printf("\033[0m\033[1m >>程序退出...\033[0m\n");
				goto END;
			case 1:
				//管理员登录
				//连接服务器，管理员登录系统
				if(AdminConnect()==0){
					strcpy(Status,"Admin");
					AdminMenu();	
				}
				else{
					printf("\033[0m\033[1m >>登录失败...\033[0m\n");	
					Pause();
				}
				break;
			case 2:
				//用户登录
				if(UserConnect()==0){
					strcpy(Status,"User");
					UserMenu();
				}
				else{
					printf("\033[0m\033[1m >>登录失败...\033[0m\n");	
					Pause();
				}

				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
	printf("\033[0m");
	return;
}
//管理员界面
void AdminMenu(){
	int choice;
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>添加用户\033[0m         \n");
		printf("/*    \033[31m 2>>删除用户\033[0m           \n");
		printf("/*    \033[31m 3>>查询用户\033[0m           \n");
		printf("/*    \033[31m 4>>修改用户\033[0m           \n");
		printf("/*    \033[31m 0>>返回上级菜单\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 0:
				//返回上级菜单
				goto END;
			case 1:
				//添加用户
				AddUsr();
				break;
			case 2:
				//删除用户
				DeletUser();
				break;
			case 3:
				//查询用户
				SearchUser();
				break;
			case 4:
				//修改用户
				UpdateUser();
				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
	printf("\033[0m");
	return;
}
//删除用户界面
void DeletUser(){
	int choice;
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>根据电话号码删除\033[0m         \n");
		printf("/*    \033[31m 2>>根据身份证号删除\033[0m           \n");
		printf("/*    \033[31m 0>>返回上级菜单\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 0:
				//返回上级菜单
				printf("\033[0m\033[1m >>程序退出...\033[0m\n");
				goto END;
			case 1:
				//更具电话号码删除
				DelUsrPhone();
				break;
			case 2:
				//更具身份证号删除
				DelUsrID();
				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
	printf("\033[0m");
	return;
}
//修改信息界面
void UpdateUser(){
	int choice;
	struct message msg;
	strcpy(msg.ip,IP);
	strcpy(msg.Status,Status);
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>根据电话号码修改\033[0m         \n");
		printf("/*    \033[31m 2>>根据身份证号修改\033[0m           \n");
		printf("/*    \033[31m 3>>根据工号修改\033[0m           \n");
		printf("/*    \033[31m 0>>返回上级菜单\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 0:
				//返回上级菜单
				printf("\033[0m\033[1m >>程序退出...\033[0m\n");
				goto END;
			case 1:
				//更具电话号码修改
				printf("\033[32m>>请输入电话号码>>\033[0m");
				scanf("%s",msg.umsg.phone);
				strcpy(msg.code,"FindUsrPhone");
				strcpy(msg.Status,"User");
				if(FindUsr(&msg)==0){
					strcpy(msg.code,"UpdatePhone");
					printf("\033[32m>>修改员工信息...\n\033[0m");
					printf("\033[32m>>请输入身份证号>>\033[0m");	
					scanf("%s",msg.umsg.ID);
					printf("\033[32m>>请输入工号>>\033[0m");	
					scanf("%s",msg.umsg.number);
					UpdateUsr(&msg);
					strcpy(msg.code,"FindUsrPhone");
					FindUsr(&msg);
				}
				else{
					Pause();
					getchar();
				}
				break;
			case 2:
				//更具身份证号修改
				printf("\033[32m>>请输入身份证号>>\033[0m");
				scanf("%s",msg.umsg.ID);
				strcpy(msg.code,"FindUsrID");
				strcpy(msg.Status,"User");
				if(FindUsr(&msg)==0){
					strcpy(msg.code,"UpdateID");
					printf("\033[32m>>修改员工信息...\n\033[0m");
					printf("\033[32m>>请输入电话号>>\033[0m");	
					scanf("%s",msg.umsg.phone);
					printf("\033[32m>>请输入工号>>\033[0m");	
					scanf("%s",msg.umsg.number);
					UpdateUsr(&msg);
					strcpy(msg.code,"FindUsrID");
					FindUsr(&msg);
				}
				else{
					Pause();
					getchar();
				}
				break;
			case 3:
				//更具工号修改
				printf("\033[32m>>请输入工号>>\033[0m");
				scanf("%s",msg.umsg.number);
				strcpy(msg.code,"FindUsrNumber");
				strcpy(msg.Status,"User");
				if(FindUsr(&msg)==0){
					strcpy(msg.code,"UpdateNumber");
					printf("\033[32m>>修改员工信息...\n\033[0m");
					printf("\033[32m>>请输入身份证号>>\033[0m");	
					scanf("%s",msg.umsg.ID);
					printf("\033[32m>>请输入电话号>>\033[0m");	
					scanf("%s",msg.umsg.phone);
					UpdateUsr(&msg);
					strcpy(msg.code,"FindUsrNumber");
					FindUsr(&msg);
				}
				else{
					Pause();
					getchar();
				}
				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
	printf("\033[0m");
	return;
}
//注册界面
void Register(){
	char buf[20]="";
	int count=0;
	int res;
	struct message msg;
	bzero(msg.buf,sizeof(msg.buf));
B:	printf("\033[32m>>输入管理员注册秘钥[默认值:\033[31mAdmin\033[32m]...\n\033[0m");	
	scanf("%s",msg.Status);
	if(strcmp(msg.Status,ADMINKEY)!=0){
		printf("\033[31m>>输入的秘钥有误...\n\033[0m");
		if(count>2){
			printf("\033[31m>>多次输入的秘钥有误...\n\033[0m");
			Pause();
			return;
		}
		count++;
		goto B;
	}
	count=0;
	printf("\033[32m>>请输入注册用户名...\n\033[0m");	
	scanf("%s",msg.user);
A:	printf("\033[32m>>请输入注册密码...\n\033[0m");	
	scanf("%s",msg.passwd);
	printf("\033[32m>>请再次输入注册密码...\n\033[0m");	
	scanf("%s",buf);
	if(strcmp(buf,msg.passwd)==0){
		//向服务器发送消息	
		strcpy(msg.passwd,buf);
		strcpy(msg.code,"Register");
		strcpy(msg.ip,IP);
		SendMsg(&msg);
		RecvMsg();
C:		res=IsExit();
		if(res==0){
			count=0;
			goto B;
		}
		else if(res==-1){
			goto C;
		}
		else{
			return;
		}
	}
	else{
		//两次密码不相同，再次输入
		printf("\033[33m>>您两次输入密码不一致，请再次输入...\n\033[0m");
		if(count>2){
			printf("\033[31m>>多次输入的密码有误...\n\033[0m");
			Pause();
			return;
		}
		count++;
		goto A;
	}
}
//用户登录界面
void UserMenu(){
	int choice;
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>查看用户\033[0m         \n");
		printf("/*    \033[31m 2>>修改用户\033[0m           \n");
		printf("/*    \033[31m 0>>返回上级菜单\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		switch(choice){
			case 0:
				//返回上级菜单
				printf("\033[0m\033[1m >>程序退出...\033[0m\n");
				goto END;
			case 1:
				//查看用户
				printf(">>%d\n",__LINE__);
				SearchUser();
				break;
			case 2:
				//修改用户
				UpdateUser();
				printf(">>%d\n",__LINE__);
				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
	printf("\033[0m");
	return;
}
//查询用户界面
void SearchUser(){
	int choice;
	struct message msg;
	strcpy(msg.ip,IP);
	strcpy(msg.Status,"Admin");
	printf("\033[0m");
	while(1){
		system("clear");
		printf("/******************************************/\n");
		printf("/*    \033[31m 1>>根据用户名查询\033[0m         \n");
		printf("/*    \033[31m 2>>根据电话号码查询\033[0m           \n");
		printf("/*    \033[31m 3>>根据工号查询\033[0m               \n");
		printf("/*    \033[31m 4>>根据身份证号号查询\033[0m               \n");
		printf("/*    \033[31m 0>>返回上级菜单\033[0m               \n");
		printf("/******************************************/\n");
		printf("\033[32m 请输入你的选项>>\033[33m");
		scanf("%d",&choice);
		getchar();
		switch(choice){
			case 0:
				//返回上级菜单
				printf("\033[0m\033[1m >>程序退出...\033[0m\n");
				goto END;
			case 1:
				//根据用户名查询
				printf("\033[32m>>请输入用户名>>\033[0m");
				scanf("%s",msg.umsg.name);
				strcpy(msg.code,"FindUsrName");
				FindUsr(&msg);
				break;
			case 2:
				//根据电话号码查询
				printf("\033[32m>>请输入电话号码>>\033[0m");
				scanf("%s",msg.umsg.phone);
				strcpy(msg.code,"FindUsrPhone");
				FindUsr(&msg);
				break;
			case 3:
				//根据工号查询
				printf("\033[32m>>请输入工号>>\033[0m");
				scanf("%s",msg.umsg.number);
				strcpy(msg.code,"FindUsrNumber");
				FindUsr(&msg);
				break;
			case 4:
				//根据身份证号查询
				printf("\033[32m>>请输入身份证号>>\033[0m");
				scanf("%s",msg.umsg.ID);
				strcpy(msg.code,"FindUsrID");
				FindUsr(&msg);
				break;
			default:
				//输入错误
				printf("\033[31m >>您的输入错误...\n\033[0m");
				Pause();
				break;
		}
	}
END:
	printf("\033[0m");
	return;
}
//任意键继续
void Pause(){
	printf("\033[34m 回车键继续...\n\033[0m");
	while(getchar()!='\n');
}
//连接服务器
int ConnectServer(){
	//socket
	int res;
	cfd=socket(AF_INET,SOCK_STREAM,0);
	if(cfd<0){
		perror("\033[31m socket error\033[0m");
		return -1;
	}
	//允许端口快速重用
	int reuse=1;
	res=setsockopt(cfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	if(res<0){
		perror("\033[31m setsockopt error\033[0m");
		return -1;	
	}
	//bind（非必须）
	
	//连接服务器；connect
	struct sockaddr_in sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(PORT);
	sin.sin_addr.s_addr=inet_addr(IP);

	res=connect(cfd,(void*)&sin,sizeof(sin));
	if(res<0){	
		perror("\033[31m connect error\033[0m");
		return -1;	
	}
	//创建线程
	/*
	pthread_t tid=-1;
	if(pthread_create(&tid,NULL,PthreadHandle,NULL)<0){
		perror("\033[31m>>创建线程失败...\n\033[0m");
		return -1;
	}
	*/
}
//关闭服务器
void DisconnectServer(){
	close(cfd);
	printf("\033[31m>>断开连接...\n\033[0m");
}
//连接服务器，管理员登录
int AdminConnect(){
	int res;
	char ch;
	struct message msg;
A:	bzero(msg.buf,MSGSIZE);
	strcpy(msg.ip,IP);
	msg.port=PORT;
	strcpy(msg.code,"AdminSignIn");
	printf("\033[32m>>请输入用户名...\n\033[0m");
	scanf("%s",msg.user);
	printf("\033[32m>>请输入登录密码...\n\033[0m");
	scanf("%s",msg.passwd);
	//向服务器发送请求
	SendMsg(&msg);
	res=RecvMsg();
	if(res==0){
		return 0;
	}
	else{
B:		printf("\033[31m>>登录失败是否继续？[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return -1;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}
	}
}
//发送消息
void SendMsg(Msg* msg){
	int res;
	do{
		res=send(cfd,msg,sizeof(Msg),0);
	}while(res<0 && errno == EINTR);

	if(res<0){
		perror("\033[31m>>无法连接服务器...\033[0m");
		return;		
	}
}
//接收消息
int RecvMsg(){
	int res;
	struct message msg;
	bzero(msg.buf,sizeof(msg.buf));
	do{
		res=recv(cfd,&msg,sizeof(Msg),0);
	}while(res<0 && errno == EINTR);

	if(res<0){
		perror("\033[31m连接服务器失败...\033[0m");
		DisconnectServer();
		return -1;		
	}
	else if(res==0){
		printf("\033[31m连接服务器超时...\033[0m");
		DisconnectServer();
	}
	else{
		res=AnalyMsg(&msg);
		if(res<0){
			return -1;
		}
		else{
			return res;
		}
	}
	return 0;
}
//解析服务器消息
int AnalyMsg(Msg* msg){
	if(strcmp(msg->code,"OK")==0){
		//注册成功
		printf("\033[33m\n服务器>>%s\033[0m",msg->buf);	
		return 0;
	}	
	if(strcmp(msg->code,"Error")==0){
		//发生错误
		printf("\033[33m\n服务器>>%s\033[0m",msg->buf);	
		return -1;
	}		
	if(strcmp(msg->code,"None")==0){
		//没有查询到对象
		printf("\033[33m\n服务器>>%s\033[0m",msg->buf);	
		return -1;	
	}		
	if(strcmp(msg->code,"AdminConnect")==0){
		//管理员登陆成功
		printf("s\033[33m\n服务器>>%s\033[0m",msg->buf);	
		return 0;	
	}		
	if(strcmp(msg->code,"UserConnect")==0){
		//用户登陆成功
		printf("s\033[33m\n服务器>>%s\033[0m",msg->buf);	
		return 0;	
	}		
	if(strcmp(msg->code,"FindInfo")==0){
		//查询的信息
		printf("\033[33m\n查询结果>>%s\033[0m",msg->buf);	
		return msg->port;
	}
}
//创建线程，监听服务器的消息
void* PthreadHandle(void* arg){
 	pthread_detach(pthread_self());
	int res;
	while(1){
		res=RecvMsgHandle();
		if(res<0){
			break;
		}
	}
	pthread_exit(NULL);
}
//线程的消息接受函数
int RecvMsgHandle(){
	int res;
	struct message msg;
	bzero(msg.buf,sizeof(msg.buf));
	do{
		res=recv(cfd,&msg,sizeof(Msg),0);
	}while(res<0 && errno == EINTR);

	if(res<0){
		perror("\033[31m连接服务器失败...\033[0m");
		DisconnectServer();
		return -1;		
	}
	else if(res==0){
		printf("\033[31m连接服务器超时...\033[0m");
		DisconnectServer();
	}
	else{
		//线程仅仅显示消息不做协议解析
		
	}
	return 0;
}
//服务器消息接收窗口
void ServerMsg(Msg* msg){
	MOV(x,y);
	printf("\033[33m\n服务器>>%s\033[0m",msg->buf);	                                
	x++;	
}
//询问是否继续
int IsExit(){
	char ch;
	printf("\033[35m>>是否返回上级菜单[Y/N]?>>\033[0m");
	getchar();
	scanf("%c",&ch);
	if(ch=='y' || ch=='Y'){
		return 1;
	}
	else if(ch=='N' || ch=='n'){
		return 0;
	}
	else{
		return -1;
	}
}

//添加用户事件
void AddUsr(){
	struct message msg;
	int res;
	char ch;
A:	//填充消息
	printf("\033[32m>>添加员工信息...\n\033[0m");
	strcpy(msg.ip,IP);
	strcpy(msg.Status,"Admin");
	strcpy(msg.code,"Add");
	printf("\033[32m>>请输入姓名>>\033[0m");	
	scanf("%s",msg.umsg.name);
	printf("\033[32m>>请输入地址>>\033[0m");	
	scanf("%s",msg.umsg.addr);
	printf("\033[32m>>请输入年龄>>\033[0m");	
	scanf("%d",&msg.umsg.age);
	printf("\033[32m>>请输入电话>>\033[0m");	
	scanf("%s",msg.umsg.phone);
	printf("\033[32m>>请输入薪资>>\033[0m");	
	scanf("%f",&msg.umsg.salary);
	printf("\033[32m>>请输入部门>>\033[0m");	
	scanf("%s",msg.umsg.part);
	printf("\033[32m>>请输入等级>>\033[0m");	
	scanf("%d",&msg.umsg.level);
	printf("\033[32m>>请输入工号>>\033[0m");	
	scanf("%s",msg.umsg.number);
	printf("\033[32m>>请输入身份证号>>\033[0m");	
	scanf("%s",msg.umsg.ID);
	strcpy(msg.user,msg.umsg.name);
	strcpy(msg.passwd,"User");

	//发送消息
	SendMsg(&msg);
	//等待接受服务器响应
	res=RecvMsg();
	if(res==0){
		//添加用户成功
		printf("\033[31m>>是否继续添加员工[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}

		return;
	}
	else{
B:		printf("\033[31m>>添加用户失败是否继续添加？[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}
	}

}
//删除用户
void DelUsrPhone(){
	struct message msg;
	int res;
	char ch;
A:	//填充消息
	printf("\033[32m>>删除员工信息...\n\033[0m");
	strcpy(msg.ip,IP);
	strcpy(msg.Status,"Admin");
	strcpy(msg.code,"DelUsrPhone");
	printf("\033[32m>>请输入员工的电话号码>>\033[0m");	
	scanf("%s",msg.umsg.phone);
	//发送消息
	SendMsg(&msg);
	//等待接受服务器响应
	res=RecvMsg();
	if(res==0){
		//添加用户成功
		printf("\033[31m>>是否继续删除员工[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}

		return;
	}
	else{
B:		printf("\033[31m>>删除用户失败是否继续删除？[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}
	}
}
//根据省份证删除用户
void DelUsrID(){
	struct message msg;
	int res;
	char ch;
A:	//填充消息
	printf("\033[32m>>删除员工信息...\n\033[0m");
	strcpy(msg.ip,IP);
	strcpy(msg.Status,"Admin");
	strcpy(msg.code,"DelUsrID");
	printf("\033[32m>>请输入员工的身份证>>\033[0m");	
	scanf("%s",msg.umsg.ID);
	//发送消息
	SendMsg(&msg);
	//等待接受服务器响应
	res=RecvMsg();
	if(res==0){
		//添加用户成功
C:		printf("\033[31m>>是否继续删除员工[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto C;
		}

		return;
	}
	else{
B:		printf("\033[31m>>查询用户失败是否继续删除？[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}
	}
}
//查询用户信息
int FindUsr(Msg* msg){
	int res,i,j;
	char ch;
	SendMsg(msg);
	//等待接受服务器响应
	res=RecvMsg();
	if(res>0){
		//查询用户成功
		for(i=0;i<res;i++){
			RecvMsg();
		}
		Pause();
		getchar();
		return 0;
	}
	else{
		printf("\033[31m>>未查询到用户>>\n\033[0m");
		Pause();
		getchar();
		return -1;
	}

}
//根据电话号码修改
void UpdateUsr(Msg* msg){
	int res;
	char ch;
	//填充消息
	printf("\033[32m>>请输入姓名>>\033[0m");	
	scanf("%s",msg->umsg.name);
	printf("\033[32m>>请输入地址>>\033[0m");	
	scanf("%s",msg->umsg.addr);
	printf("\033[32m>>请输入年龄>>\033[0m");	
	scanf("%d",&msg->umsg.age);
	printf("\033[32m>>请输入薪资>>\033[0m");	
	scanf("%f",&msg->umsg.salary);
	printf("\033[32m>>请输入部门>>\033[0m");	
	scanf("%s",msg->umsg.part);
	printf("\033[32m>>请输入等级>>\033[0m");	
	scanf("%d",&msg->umsg.level);
	//发送消息
	SendMsg(msg);
	//等待接受服务器响应
	res=RecvMsg();
	if(res==0){
		//修改用户成功
		printf("\033[31m>>修改员工信息成功\n\033[0m");
		Pause();
		getchar();
		return;
	}
	else{
		printf("\033[31m>修改用户失败\n\033[0m");
		Pause();
		getchar();
	}
}
//修改信息界面
void UserUpdate(){
	int res;
	struct message msg;
	strcpy(msg.ip,IP);
	strcpy(msg.Status,"User");
	strcpy(msg.code,"UserUpdate");

	char ch;
	//填充消息
	printf("\033[32m>>请输入姓名>>\033[0m");	
	scanf("%s",msg.umsg.name);
	printf("\033[32m>>请输入地址>>\033[0m");	
	scanf("%s",msg.umsg.addr);
	printf("\033[32m>>请输入年龄>>\033[0m");	
	scanf("%d",&msg.umsg.age);
	printf("\033[32m>>请输入薪资>>\033[0m");	
	scanf("%f",&msg.umsg.salary);
	printf("\033[32m>>请输入部门>>\033[0m");	
	scanf("%s",msg.umsg.part);
	printf("\033[32m>>请输入等级>>\033[0m");	
	scanf("%d",&msg.umsg.level);
	printf("\033[32m>>请输入工号>>\033[0m");	
	scanf("%s",msg.umsg.number);
	printf("\033[32m>>请输入身份证号>>\033[0m");	
	scanf("%s",msg.umsg.ID);
	printf("\033[32m>>请输入电话号>>\033[0m");	
	scanf("%s",msg.umsg.phone);
	//发送消息
	SendMsg(&msg);
	//等待接受服务器响应
	res=RecvMsg();
	if(res==0){
		//修改用户成功
		printf("\033[31m>>修改信息成功\n\033[0m");
		Pause();
		getchar();
		return;
	}
	else{
		printf("\033[31m>修改信息失败\n\033[0m");
		Pause();
		getchar();
	}
}
//查询用户界面
void UserFind(){


}
//用户登录
int UserConnect(){
	int res;
	char ch;
	struct message msg;
A:	bzero(msg.buf,MSGSIZE);
	strcpy(msg.ip,IP);
	strcpy(msg.Status,"User");
	msg.port=PORT;
	strcpy(msg.code,"UserSignIn");
	printf("\033[32m>>请输入用户名...\n\033[0m");
	scanf("%s",msg.user);
	printf("\033[32m>>请输入登录密码(\033[31m默认密码：User\033[0m)...\n\033[0m");
	scanf("%s",msg.passwd);
	//向服务器发送请求
	SendMsg(&msg);
	res=RecvMsg();
	if(res==0){
		return 0;
	}
	else{
B:		printf("\033[31m>>登录失败是否继续？[Y/N]>>\033[0m");
		getchar();
		scanf("%c",&ch);
		getchar();
		if(ch=='Y'||ch=='y'){
			goto A;
		}
		else if(ch=='N'||ch=='n'){
			return -1;
		}
		else{
			printf("\033[31m>>输入错误...\n\033[0m");
			goto B;
		}
	}
}
