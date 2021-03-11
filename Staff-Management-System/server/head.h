/*******************************************************************************
 * head.h
 *员工管理系统
 * @fcopyright  Copyright 2021 SIC Microelectronics CO. Ltd.
 * @fauthor     [JiangYing] [2405596758@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2021-03-08] Create file.
 ******************************************************************************/
#ifndef __HEAD_H__
#define __HEAD_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sqlite3.h>

#define IP "0.0.0.0"
#define PORT 2021
#define SIZE 128
#define MAX_USER_COUNT 10
#define EVENT_COUNT 8
#define MSGSIZE 1024


typedef struct UsrMsg{
	char name[20];    
	char addr[20];    
	int age;          
	char phone[20];   
	float salary;     
	char part[20];    
	int level;        
	char number[20];  
	char ID[20];      
}UMsg;                

typedef struct message{
	char ip[20];
	int port;
	char Status[20];
	char code[20];
	char user[20];
	char passwd[20];
	char buf[MSGSIZE];
	struct UsrMsg umsg;
}__attribute__((packed)) Msg;

typedef struct clinfo{
	int epfd;
	int fd;
	struct epoll_event events[EVENT_COUNT];
}cinfo;
extern sqlite3* db;
extern int sfd;
extern char user[20];

//启动服务器
int ServerInit();
//服务器epoll主函数
int EpollMain(cinfo* info,int sfd);
//新的连接事件
void NewConnect(cinfo* info);
//旧的连接读事件
void Read(cinfo* info,int i);
//打开数据库
int SqliteInit();
//关闭数据库
void SqliteClose();
//解析消息协议
void AnalyMsg(Msg* msg,int cfd);
//管理员登录事件
void AdminSignIn(Msg* msg,int cfd);
//向客户端发送消息
void Send(Msg* msg,int cfd);
//注册事件
void SignUp(Msg* msg,int cfd);
//注册函数的回调函数
int AdminSignInHandle(void* argi,int f_num, char** f_value,char** f_name);
//添加用户
void AddUser(Msg* msg,int cfd);
//删除用户
void DelUser(Msg* msg,int cfd);
//查询用户
void FindUser(Msg* msg,int cfd);
//更新用户
void UpdateUser(Msg* msg,int cfd);
//用户登录事件
void UserSignIn(Msg* msg,int cfd);

#endif //__HEAD_H__
