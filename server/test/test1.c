/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  检测错误函数
 *
 *        Version:  1.0
 *        Created:  2015年01月29日 23时14分01秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *           Rule:  ???? question 
 *                  [    comment
 *
 * =====================================================================================
 */


#include<stdio.h>
#include<error.h>

void MyError(char * err_string,const char * function, int line)
{
        fprintf(stderr,"[function:%s,line:%d]",function,line);
        perror(err_string);
}


int main()
{
        MyError("Sting\n",__FUNCTION__,__LINE__);
}
