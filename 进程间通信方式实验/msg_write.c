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

//����Ϣ�������ʽ����
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

    //������Ϣ���У����Ѿ�������ֱ�ӻ�ȡ
	//key:��ֵ��һ�������ı�ʶ����ʶһ��ͨѶͨ��
    msgid=msgget((key_t)1234,0666|IPC_CREAT);
    if(msgid==-1)
    {
        cout<<"msgget faild with error";
        exit(EXIT_FAILURE);
    }

    //����Ϣ������д����Ϣ��֪��д��end
    while(1)
    {
        cout<<"enter some text:"<<endl;

        //������Ϣ
        fgets(buff,max_v,stdin);
        data.type=1;
        strcpy(data.mtext,buff);

        //д����Ϣ 	int msgsnd(int msqid,const void *msgp,size_t msgsz,int msgflag)
		//msgp��ָ����Ϣ��������ָ�룬������ʱ�洢�ͷ�����Ϣ����һ���û��ɶ����ͨ�ýṹ
        if(msgsnd(msgid,(void*)&data,max_v,0)==-1)
        {
            cout<<"msgsnd faild with error"<<endl;
            exit(EXIT_FAILURE);
        }

        //end����
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}