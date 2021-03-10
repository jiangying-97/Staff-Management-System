/*******************************************************************************
 * head.h
 * 员工管理系统
 * @fcopyright  Copyright 2021 SIC Microelectronics CO. Ltd.
 * @fauthor     [JiangYing] [2405596758@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2021-03-08] Create file.
 ******************************************************************************/
#ifndef __HEAD_H__
#define __HEAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define PORT 2021
#define IP "192.168.131.131"
#define MSGSIZE 1024
#define ADMINKEY "Admin"
#define MOV(x,y) printf("\033[%d;%dH",(y),(x))
extern int x;
extern int y;
extern int cfd;
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

//主界面
void MainMenu();
//登录界面
void SignInMenu();
//管理员登录界面
void AdminMenu();
//注册界面
void Register();
//用户登录界面
void UserMenu();
//删除用户界面
void DeletUser();
//修改信息界面
void UpdateUser();
//查询用户界面
void SearchUser();
//任意键继续
void Pause();
//连接服务器
int ConnectServer();
//关闭服务器
void DisconnectServer();
//连接服务器，管理员登录
int AdminConnect();
//发送消息
void SendMsg(Msg* msg);
//接收消息
int RecvMsg();
//解析服务器消息
int AnalyMsg(Msg* msg);
//创建线程，监听服务器的消息
void* PthreadHandle(void* arg);
//线程的消息接受函数
int RecvMsgHandle();
//服务器消息接收窗口
void ServerMsg(Msg* msg);
//询问是否继续
int IsExit();
//添加用户事件
void AddUsr();
//根据电话号码删除用户
void DelUsrPhone();
//根据省份证号删除用户
void DelUsrID();
//查询用户信息
int FindUsr(Msg* msg);
//修改
void UpdateUsr(Msg* msg);
//用户修改
void UserUpdate();
//用户登录
int UserConnect();


#endif //__HEAD_H__
