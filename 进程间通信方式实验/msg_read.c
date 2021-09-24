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

  //�ҵ���Ϣ����
  msgid=msgget((key_t)1234,0666|IPC_CREAT);
  if(msgid==-1)
  {
    cout<<"msgget function faild with error"<<endl;
    exit(EXIT_FAILURE);
  }

  //�Ӷ����л�ȡ��Ϣ��ֱ������end
  //��ȡ��Ϣ ssize_t msgrcv(int msqid,void *msgp,size_t msgsz,int msgflag)
  //msgp��ָ����Ϣ��������ָ�룬������ʱ�洢�ͷ�����Ϣ����һ���û��ɶ����ͨ�ýṹ
  while(1)
  {
    if(msgrcv(msgid,(void*)&data,max_v,msgtype,0)==-1)
    {
      cout<<"msgrcv failed with error"<<endl;
      exit(EXIT_FAILURE);
    }

    //��ӡһ��
    cout<<"you get message:"<<data.mtext<<endl;

    if(strncmp(data.mtext,"end",3)==0)
    {
      return 0;
    }
  }


//������Ϣ�������� int msgctl(int msgqid,int cmd,struct msqid_ds *buf)
  //ɾ����Ϣ����
  if(msgctl(msgid,IPC_RMID,0)==-1)
  {
    cout<<"msgctl IPC_RMID faild error"<<endl;
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}