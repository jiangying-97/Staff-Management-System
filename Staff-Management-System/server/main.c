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
	system("clear");
	printf("\033[34m/**************服务器启动*****************/\n\033[0m");
	if(ServerInit()<0){
		printf("\033[31m 服务器启动失败...\n \033[0m");
		return -1;
	}
	SqliteClose();
	printf("\033[1m/**************服务器关闭*****************/\n\033[0m");
	return 0;
}
