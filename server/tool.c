/*
 * =====================================================================================
 *
 *       Filename:  tool.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年01月30日 16时40分20秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *           Rule:  ???? question 
 *                  [    comment
 *
 * ===================================================================================== */
#include"tool.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  MyError
 *  Description:  输出错误并且退出
 *        Entry:  错误描述，函数，位置
 *         Exit:  void 
 * =====================================================================================
 */
void MyError(char * err_string,const char * function, int line)
{
        fprintf(stderr,"[function:%s,line:%d]",function,line);
        perror(err_string);
        exit(1);

}
void MyErrorPthread(char * err_string,const char * function, int line)
{
        fprintf(stderr,"[function:%s,line:%d]",function,line);
        perror(err_string);
        pthread_exit(0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Log
 *  Description:  建立日志文件 logfile ,不存在则创建，按照
 *                时间 事件 人物 的格式写入
 *        Entry:  操作和使用者
 *         Exit:  空
 * =====================================================================================
 */
void Log(const char *log_string, const char * user_string)
{   
        FILE * log_fp;
        time_t time_now = time(NULL);        

        if( (log_fp=fopen("log_fp","a+")) == NULL )
        {
                MyError("fopne",__FUNCTION__,__LINE__ );
        }
        fprintf(log_fp,"time:%s what:%10s   who:%10s\n",ctime(&(time_now)),log_string,user_string);
        fclose(log_fp);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  FdToUsername
 *  Description:  根据套接字，找到用户名
 *        Entry:  套接字
 *         Exit:  用户名
 * =====================================================================================
 */
const char * FdToUsername( int client_fd )
{
        PtrUserDate temp;

        temp =  g_user_list->next;

        while( temp != NULL )
        {
                if( temp->confd == client_fd )
                {
                        return temp->name;
                }
                temp = temp->next;
        }
        return NULL;
}

int Mywrite( int fd, const void * buf,unsigned int count )
{
        int return_value;
        if( (return_value=write(fd,buf,count)) > 0 )
        {
                return return_value; 
        }
        else{
                printf("write 函数出错 \n");
                exit(0);
        }

  
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_info
 *  Description:  获取一行输入,以回车为结尾，在buf中删除\n,以\0为结尾
 *        Entry:  存放空间 buf ,空间大小
 *         Exit:  成功存储返回 0 ， buf空间不存，存储失败在返回 -1 ， 字符串过长返回 -2
 * =====================================================================================
 */
int   GetInfo( char *buf,unsigned int counnt)
{
        int  i = 0;
        int  c;
        int temp;

        if( buf == NULL )
        {
                return -1;
        }
         while( (temp=getchar()) == '\n') ;
        
        buf[i++] = temp;

         while((buf[i]=getchar()) != '\n')
         {
                 i++;
         }
        if( i >= counnt )
        {
                memset(buf,0,counnt);
                return -2;
        }

         buf[i] = '\0';//去回车，加\0
         return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DealCommond
 *  Description:  解析输入命令的字符串
 *        Entry:  命令的字符串commond
 *         Exit: 成功， 返回有几个命令【以空格区分】，而 commond_conist 以二位字符数组的形式依次存放命令
 *               失败返回 -1
 *
 * =====================================================================================
 */
int DealCommond(char *commond, char (* commond_conist)[USER_MAX])
{
        int count=0;
        int i=0;
        int j=0;
        char temp[USER_MAX];
        if( commond == NULL||commond_conist==NULL)
                return -1;
        
        while( commond[i]==' ' )  i++;
        while(1)
        {
                temp[j] = commond[i];
                if( commond[i] == ' '|| commond[i] == '\0' )
                {
                        temp[j] = '\0';
                        strcpy(*(commond_conist+count),temp);
                        j = -1;
                        count++;
                        if( commond[i] == '\0' )
                        {
                                return count;
                        }//字符串结束，结束函数，返回个数
                        while( commond[i]== ' ' )  i++;
                        i--;//消去 判断while结束 的影响
                }//遇到一个字符段
                j++;
                i++;

        }//直接字符串结束
}
