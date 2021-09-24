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
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>

using namespace std;

#define sem_key 4001//�ź���key
#define shm_key 5678//�����ڴ�key

union semnu
{
  int val;
};

int main()
{

  //���������ڴ�
  int shmid=shmget(shm_key,sizeof(int),IPC_CREAT|0666);
  if(shmid<0)
  {
    cout<<"create shm error";
    return -1;
  }

  //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�
  void *shmptr=shmat(shmid,NULL,0);
  if(shmptr==(void*)-1)
  {
    cout<<"shmat error:"<<strerror(errno)<<endl;
    return -1;
  }

  int *data=(int*)shmptr;
  //�����ʹ��ź�����semget����
  
  int semid=semget(sem_key,2,0666);//�����ź���

  union semnu s;
  struct sembuf sbuf;
  struct sembuf sb[1] = {{0, 1, 0}};

  while(1)
  {
    //sleep(1);
    cin>>*data;//�������ڴ�д������
	
//�ı��ź�����ֵ��semop���� int semop(int semid,struct sembuf *sops,unsigned nsops);	

// sem_id:�ź�����ʶ��
// *sops��ָ��洢�źŲ����ṹ������ָ��
// nsops:�źŲ����ṹ��������>=1

    semop(semid, sb, 1);//�ź���+1
  }
  return 0;
}