/*******************************************************************************
 * main.c
 *员工管理系统
 * @fcopyright  Copyright 2021 SIC Microelectronics CO. Ltd.
 * @fauthor     [JiangYing] [2405596758@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2021-03-08] Create file.
 ******************************************************************************/
#include "head.h"

int main(int argc,const char argv[]){
	//连接服务器
	int res;
	res=ConnectServer();
	if(res<0){
		printf("\033[31m 连接服务器失败...\n\033[0m");
		return -1;
	}
	else{	
		printf("\033[32m 连接服务器成功...\n\033[0m");
	}
	//打开菜单
	MainMenu();
	//关闭服务器
	DisconnectServer();
	return 0;
}
