/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  服务器基本函数定义和命令宏定义
 *
 *        Version:  1.0
 *        Created:  2015年01月28日 21时15分18秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *操
 * =====================================================================================
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include"frame.h"
#include"user.h"
#include"debug.h"
#include"tool.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>    // 系统调用的封装
#include<pthread.h>   //线程
#include<sys/types.h> //定义基本数据类型
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h> //字节顺序和网络地址的转换

/*          服务器基本参数         */

#define         SERV_PORT       4507      /*服务器端口号*/
#define         LISTENQ         12        /*连接请求的最大队列长度*/


void SloveConnect(void);
void InitServer(void);
void DealDate(void);
void LogOrRegist( int clie_fd,const struct CliToSerFrame * data );



#endif

