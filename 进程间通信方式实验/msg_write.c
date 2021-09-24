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

//以消息链表的形式出现
struct msg_st
{
    long type;//>0
    char mtext[max_v];
};

int main()
{
    struct msg_st data;
    char buff[max_v];
    int msgid=-1;

    //建立消息队列，若已经存在则直接获取
	//key:键值，一个公共的标识来标识一个通讯通道
    msgid=msgget((key_t)1234,0666|IPC_CREAT);
    if(msgid==-1)
    {
        cout<<"msgget faild with error";
        exit(EXIT_FAILURE);
    }

    //向消息队列中写入信息，知道写入end
    while(1)
    {
        cout<<"enter some text:"<<endl;

        //输入信息
        fgets(buff,max_v,stdin);
        data.type=1;
        strcpy(data.mtext,buff);

        //写入信息 	int msgsnd(int msqid,const void *msgp,size_t msgsz,int msgflag)
		//msgp：指向消息缓冲区的指针，用来暂时存储和发送消息，是一个用户可定义的通用结构
        if(msgsnd(msgid,(void*)&data,max_v,0)==-1)
        {
            cout<<"msgsnd faild with error"<<endl;
            exit(EXIT_FAILURE);
        }

        //end跳出
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}