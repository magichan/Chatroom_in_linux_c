/*
 * =====================================================================================
 *
 *       Filename:  user.c
 *
 *    Description:  服务器用户维护代码模块 
 *
 *        Version:  1.0
 *        Created:  2015年01月28日 22时11分48秒
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
#include"user.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InitUserList
 *  Description:  由文件"userlist",初始化用户列表[带头节点]，不存在该文件则创建
 *        Entry:  void 
 *         Exit:  链表头节点
 * =====================================================================================
 */
PtrUserDate InitUserList( void )           
{
        return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  AddUser
 *  Description:  添加用户
 *        Entry:  链表头节点 和 需要添加节点的指针
 *         Exit:  成功返回 0 失败返回1
 * =====================================================================================
 */
int  AddUser( PtrUserDate  list, PtrUserDate Node ) 
{
        return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DeleUser
 *  Description:  删除一个用户信息
 *        Entry:  链表头节点 和  需要删除的用户名，如果char 指向 NULL 删除全部用户
 *         Exit:  成功返回 0 失败返回 1 
 * =====================================================================================
 */
int  DeleUser( PtrUserDate list, char * name ) 
{
        return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  PtrUserDate
 *  Description:  根据 用户名，查找用户信息
 *        Entry:  链表头 和 用户名
 *         Exit:  成功返回用户信息 失败返回NULL
 * =====================================================================================
 */
const PtrUserDate  SearchUser( PtrUserDate list, char * name )
{
        return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  WriteUserList
 *  Description:  将列表写入文件 "userlist"
 *        Entry:  链表头
 *         Exit:  成功返回 0 失败返回1 
 * =====================================================================================
 */
int  WriteUserList( PtrUserDate list)    
{
        return 0;
}
