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
//允许两个不相关的进程访问同一个逻辑内存

//自定义数据单元
struct shared_use_st
{
    int flag;//0代表可写，1代表可读
    char text[max_v];
};

int main()
{
    //创建共享内存
    int shmid=shmget((key_t)1234,sizeof(struct shared_use_st),0666|IPC_CREAT);
    if(shmid==-1)
    {
        cout<<"shmget failed with error"<<endl;
        exit(EXIT_FAILURE);
    }

    void *share_memory=(void*)0;

    //将共享内存链接到本进程的地址空间
    share_memory=shmat(shmid,(void *)0,0);
    if(share_memory==(void*)-1)
    {
        cout<<"shmat failed with error"<<endl;
        exit(EXIT_FAILURE);
    }
    //打印共享内存在进程数据段的地址
    cout<<"memory attached at "<<(long)share_memory<<endl;

    struct shared_use_st *shared_stuff=(struct shared_use_st *)share_memory;
    shared_stuff->flag=0;
    char buff[max_v];

    while(1)
    {
        //当前共享内存处于不可读写状态，等待
        if(shared_stuff->flag==1)
        {
            sleep(1);
            cout<<"wait for client..."<<endl;
        }

        //输入
        cout<<"exter some text"<<endl;
        fgets(buff,max_v,stdin);
        strncpy(shared_stuff->text,buff,max_v);
        shared_stuff->flag=1;

        //遇到end跳出
        if(strncmp(buff,"end",3)==0)
        {
            break;
        }
    }

    //将共享内存从本进程地址空间分离
    if(shmdt(share_memory)==-1)
    {
        cout<<"shmdt failed with error"<<endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}