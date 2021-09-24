#include <iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/msg.h>
using namespace std;

#define max_v 512
struct msg_st
{
  long type;//>0
  char mtext[max_v];
};

int main()
{
  int msgid=-1;
  struct msg_st data;
  long int msgtype=0;

  //找到消息队列
  msgid=msgget((key_t)1234,0666|IPC_CREAT);
  if(msgid==-1)
  {
    cout<<"msgget function faild with error"<<endl;
    exit(EXIT_FAILURE);
  }

  //从队列中获取消息，直到遇到end
  //读取消息 ssize_t msgrcv(int msqid,void *msgp,size_t msgsz,int msgflag)
  //msgp：指向消息缓冲区的指针，用来暂时存储和发送消息，是一个用户可定义的通用结构
  while(1)
  {
    if(msgrcv(msgid,(void*)&data,max_v,msgtype,0)==-1)
    {
      cout<<"msgrcv failed with error"<<endl;
      exit(EXIT_FAILURE);
    }

    //打印一下
    cout<<"you get message:"<<data.mtext<<endl;

    if(strncmp(data.mtext,"end",3)==0)
    {
      return 0;
    }
  }


//设置消息队列属性 int msgctl(int msgqid,int cmd,struct msqid_ds *buf)
  //删除消息队列
  if(msgctl(msgid,IPC_RMID,0)==-1)
  {
    cout<<"msgctl IPC_RMID faild error"<<endl;
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}