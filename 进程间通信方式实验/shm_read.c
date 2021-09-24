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

    while(1)
    {
        //��д
        if(shared_stuff->flag)
        {
            cout<<"you wrote :"<<shared_stuff->text<<endl;
            sleep(1);
            shared_stuff->flag=0;
            
            //����end����
            if(strncmp(shared_stuff->text,"end",3)==0)
            {
                break;
            }
        }
    }
    
    //�������ڴ�ӱ����̵�ַ�ռ����
    if(shmdt(share_memory)==-1)
    {
        cout<<"shmdt failed with error"<<endl;
        exit(EXIT_FAILURE);
    }
    
    //ɾ�������ڴ�
    if(shmctl(shmid,IPC_RMID,0))
    {
        cout<<"shmctl IPC_RMID faild with error";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}