#include"header.h"
#define CALPORT_PROCESS  8999	//�������ķ������˿ں�
#define LISQUEUE   20	//�����������Ķ��г���
#define BUFFER_SIZE 1024	//��д��Ϣ����������


char sendbuf[BUFFER_SIZE]={0};
char recvbuf[BUFFER_SIZE]={0};

int connfd;///�Ϳͻ��˵�����
int server_sockfd;///����sockfd


void main(int argc,char *argv[]) {
    ///����sockfd
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///����sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(CALPORT_PROCESS);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind���ɹ�����0��������-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1) {
        perror("Bind error happens!!!");
        exit(1);
    }

    ///listen���ɹ�����0��������-1
    if(listen(server_sockfd,LISQUEUE) == -1) {
        perror("Listen error happens!!!");
        exit(1);
    }
	
	printf("Waiting for connection!!!\n");

	
	while(1){
		int n;
		//�ͻ����׽���
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);

		//����ͻ������ӣ��ɹ����طǸ������֣�������-1
		connfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		
		memset(recvbuf,0,sizeof(recvbuf));
		memset(sendbuf,0,sizeof(sendbuf));
		
		int pid = fork();
        if(pid == 0){  //����һ���ӽ��̴�������
            close(server_sockfd);//�ر��ӽ��̶˵ļ���        
            while(1){
                /*��connfd�ļ�������ָ����ļ���ȡ����*/
                n  = read(connfd, recvbuf, sizeof(recvbuf));
                if(n == 0){
                    printf("No recv from client!!!\n");
					break;
                }
                printf("recv msg:%s\n",recvbuf);

                /*��bufת��Ϊ��д��д��connfd*/        
                double a = 0, b = 0;
				char c;
				sscanf(recvbuf, "%lf%c%lf", &a, &c, &b);
 
				if ('+' == c) {
					sprintf(sendbuf,"%lf", a+b);
				}	
				else if ('-' == c) {
					sprintf(sendbuf, "%lf", a-b);
				}
				else if ('*' == c) {
					sprintf(sendbuf, "%lf", a*b);
				}
				else if ('/' == c) {
					sprintf(sendbuf, "%lf", a/b);
				}	
				printf("sendMsg=%s\n",sendbuf);
				write(connfd, sendbuf, strlen(sendbuf));
				}    
				close(connfd);//������Ϲر�����
				exit(0);
        }else if(pid > 0){ //������
            close(connfd);//�������ӽ��̴��������̹ر�
        }else{
            printf("Fork error happens!!!\n");
        }

        if(n == 0 ||  n == -1){
            break;
        }
    }
    return 0;
		
		
		

}

			
 

