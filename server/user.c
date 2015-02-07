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
#include"tool.h"
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
        FILE * fp;
        PtrUserDate  temp;
        PtrUserDate  head;
        PtrUserDate  p;
        if( (fp=fopen("userlist","ab+"))== NULL )
        {
          MyError("fopen",__FUNCTION__,__LINE__);
        }
        head = ( PtrUserDate )malloc(sizeof(struct UserDate));
        head->next = NULL;
        p = head;

        temp = ( PtrUserDate )malloc(sizeof(struct UserDate));
        while( fread(temp,sizeof(struct UserDate),1,fp) >0 )
        {
                temp->next = NULL;
                p->next = temp;
                p = temp;//挂链

                p->status = USER_STATUS_DOWN;
                p->confd  = 0;//初始化状态
                temp = ( PtrUserDate )malloc(sizeof(struct UserDate));
        }
        free(temp);

        return head;
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
        PtrUserDate p;
        if( Node == NULL )
        {
          MyError("Adduser",__FUNCTION__,__LINE__);
        }
        p = list;
        while( p->next!=NULL )
        {
                p= p->next;
        }
        p->next = Node;
        return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DeleUser
 *  Description:  删除一个用户信息
 *        Entry:  链表头节点 和  需要删除的用户名，如果char 指向 NULL 删除全部用户[慎用]
 *         Exit:  成功返回 0 失败返回 1 
 * =====================================================================================
 */
int  DeleUser( PtrUserDate list, char * name ) 
{
        PtrUserDate p,q;
        if( name == NULL )
        {
                p = list->next;
                while( p!=NULL )
                {
                        q = p;
                        p = p->next;
                        free(q);
                }
                return 0;
        
        }// 全部删除
        for( p=list,q=list->next;q!=NULL&&strcmp(q->name,name)!=0;p=p->next,q=q->next)
        {
                ;
        }
        if( q==NULL )
        {
                return 1;
        }
        p->next = q->next;
        free(q);
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

 PtrUserDate  SearchUser( PtrUserDate list, const char * name )
        /* 查找用户 */
{
        PtrUserDate p;
        p = list->next;
        while( p!=NULL )
        {
                if( strcpy(p->name,name)==0 )
                        return p;
                p = p->next;
        }
        return p;
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
        FILE *fp;
        PtrUserDate p;
        if( (fp=fopen("userlist","w")) == NULL )
        {
                MyError("fopne",__FUNCTION__,__LINE__);
        }
        p = list->next;
        while( p!=NULL )
        {
                fwrite(p,sizeof(struct UserDate),1,fp);
                p = p->next;
        }
        return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  UserListIsEmpty
 *  Description:  判断用户表是否为空
 *        Entry:  用户表头
 *         Exit:  为空返回 1
 *                 非空返回 0
 * =====================================================================================
 */
int  UserListIsEmpty( PtrUserDate list)
{
        if( list->next==NULL)
                return 1;
        else
                return 0;
}
