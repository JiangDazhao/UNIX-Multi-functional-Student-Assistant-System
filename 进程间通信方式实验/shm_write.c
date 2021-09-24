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
#include<sys/shm.h>
using namespace std;

#define max_v 2048
//������������صĽ��̷���ͬһ���߼��ڴ�

//�Զ������ݵ�Ԫ
struct shared_use_st
{
    int flag;//0�����д��1����ɶ�
    char text[max_v];
};

int main()
{
    //���������ڴ�
    int shmid=shmget((key_t)1234,sizeof(struct shared_use_st),0666|IPC_CREAT);
    if(shmid==-1)
    {
        cout<<"shmget failed with error"<<endl;
        exit(EXIT_FAILURE);
    }

    void *share_memory=(void*)0;

    //�������ڴ����ӵ������̵ĵ�ַ�ռ�
    share_memory=shmat(shmid,(void *)0,0);
    if(share_memory==(void*)-1)
    {
        cout<<"shmat failed with error"<<endl;
        exit(EXIT_FAILURE);
    }
    //��ӡ�����ڴ��ڽ������ݶεĵ�ַ
    cout<<"memory attached at "<<(long)share_memory<<endl;

    struct shared_use_st *shared_stuff=(struct shared_use_st *)share_memory;
    shared_stuff->flag=0;
    char buff[max_v];

    while(1)
    {
        //��ǰ�����ڴ洦�ڲ��ɶ�д״̬���ȴ�
        if(shared_stuff->flag==1)
        {
            sleep(1);
            cout<<"wait for client..."<<endl;
        }

        //����
        cout<<"exter some text"<<endl;
        fgets(buff,max_v,stdin);
        strncpy(shared_stuff->text,buff,max_v);
        shared_stuff->flag=1;

        //����end����
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
    }

    //�������ڴ�ӱ����̵�ַ�ռ����
    if(shmdt(share_memory)==-1)
    {
        cout<<"shmdt failed with error"<<endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}