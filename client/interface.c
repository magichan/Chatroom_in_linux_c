/*
 * =====================================================================================
 *
 *       Filename:  interface.c
 *
 *    Description
 *
 *        Version:  1.0
 *        Created:  2015年02月06日 17时34分13秒
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
#include"interface.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InterfaceWelocme
 *  Description:  输出初始界面
 *        Entry:
 *         Exit:
 */
void InterfaceWelcome( void )
{
        printf("******HELLO*****\n");
        printf("*    1.登录    *\n");
        printf("*    2.注册    *\n");
        printf("*    3.退出    *\n");
        printf("****************\n");
        
}
void InterfaceBye(void )
{
        printf("******66666*****\n");
        printf("*      BYE     *\n");
        printf("****************\n");
}
void InterfaceRegistSucces(void)
{
        printf("******6666666*****\n");
        printf("*     注册成功   *\n");
        printf("*退出后请重新登录*\n");
        printf("********××********\n");

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InterfaceHello
 *  Description:  输出登陆成功后的界面
 *        Entry:
 *         Exit:
 * =====================================================================================
 */
void InterfaceHello( void )
{
        
        printf("******6666666*****\n");
        printf("*     登陆成功   *\n");
        printf("*   输入 ::help  *\n");
        printf("*    寻求帮助    *\n");
        printf("********××********\n");
}
