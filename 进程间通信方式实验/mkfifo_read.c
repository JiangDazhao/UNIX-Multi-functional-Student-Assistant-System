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
using namespace std;

//�����ܵ�����ͨ��·������ָ��,�������ļ�ϵͳ���ǿɼ���
//�����������ܵ�֮���������̾Ϳ��԰���������ͨ�ļ�һ�����ж�д������ʹ�÷���
//�����ϸ���ѭFIFO�Ƚ��ȳ�ԭ��
#define P_FIFO "/tmp/p_fifo" 
#define max_v 105

int main()
{
  char str[max_v];

  if(access(P_FIFO,F_OK)==0)//�ܵ��ļ�����
  {
    execlp("rm","-f",P_FIFO,NULL);//ɾ��
    cout<<"the FIFO file have existed,delete it";
  }

//�����ܵ��Ĵ�����int mkfifo��const char* pathname��mode_t mode��

  if(mkfifo(P_FIFO,0777)<0)//���������ܵ���fifo�ļ�
  {
    cout<<"create fifo file error"<<endl;
    return 0;
  }

  int fd=open(P_FIFO,O_RDONLY|O_NONBLOCK);//��fifo�ļ�

  while(1)
  {
    memset(str,0,sizeof(str));//���
    int num=read(fd,str,max_v);//��ȡ����
    if(num==0)
    {
      cout<<"no data"<<endl;
    }else
    {
      str[num]='\0';
      cout<<str<<endl;
    }
    sleep(1);//sleep 1s������
  }
  close(fd);//�ر�fd
  return 0;
}