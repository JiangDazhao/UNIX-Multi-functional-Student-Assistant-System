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

    while(1)
    {
        //可写
        if(shared_stuff->flag)
        {
            cout<<"you wrote :"<<shared_stuff->text<<endl;
            sleep(1);
            shared_stuff->flag=0;
            
            //遇到end跳出
            if(strncmp(shared_stuff->text,"end",3)==0)
            {
                break;
            }
        }
    }
    
    //将共享内存从本进程地址空间分离
    if(shmdt(share_memory)==-1)
    {
        cout<<"shmdt failed with error"<<endl;
        exit(EXIT_FAILURE);
    }
    
    //删除共享内存
    if(shmctl(shmid,IPC_RMID,0))
    {
        cout<<"shmctl IPC_RMID faild with error";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}