/*
 * =====================================================================================
 *
 *       Filename:  tool.h
 *
 Description:  提供一些工具和日志函数
 *
 *        Version:  1.0
 *        Created:  2015年01月30日 16时37分09秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
/* 日志的宏定义 */
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<stdlib.h>
#include"frame.h"
void MyError(char * err_string,const char * function, int line);
void MyErrorPthread(char * err_string,const char * function, int line);
void Log(const char *log_string, const char * user_string);
int  GetInfo( char *buf,unsigned int counnt);
void Myfflush(void);
int DealCommond(char *commond, char (* commond_conist)[USER_MAX]);
