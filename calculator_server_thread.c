#include"header.h"
#define CALPORT_THREAD  9000	//�������ķ������˿ں�
#define LISQUEUE   20	//�����������Ķ��г���
#define BUFFER_SIZE 1024	//��д��Ϣ����������
#define THREAD_POOL_SIZE      ( ( const int )20 )//�̳߳ش�С

static volatile int g_run = 1;
static volatile int g_pool_run = 1;
static volatile int g_send = 0;

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

char sendbuf[BUFFER_SIZE]={0};
char recvbuf[BUFFER_SIZE]={0};

int connfd;///�Ϳͻ��˵�����
int server_sockfd;///����sockfd

static void sig_handler( int sig )	//terminate�ⲿ�źŹر��̳߳�
{
	if ( sig == SIGTERM )g_run = 0;
}

static void task_dispatch( )  //����ַ�����
{
		pthread_mutex_lock( &g_mutex );
		g_send ++;
	//	printf("dispatch_gsend:%d\n",g_send);
		pthread_mutex_unlock( &g_mutex );
		//���������߳����������ˣ����������߳�
		pthread_cond_broadcast( &g_cond );
		usleep( 10 );
}

void* thr_routine( void* arg )	//������̴߳�����
{
	//�ͻ����׽���
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	//����ͻ������ӣ��ɹ����طǸ������֣�������-1
	connfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
	if(connfd<0) {
		perror("Connect error happens!!!");
		exit(1);
	}
	else{
		task_dispatch();
	}
		
	while ( 1 )
	{	
		pthread_mutex_lock( &g_mutex );
		
		while ( g_pool_run == 1 && g_send == 0 )
		{
			/*�̳߳��ڹ�������û������ʱ�������߳�һֱ�ȴ�����g_cond�ź�*/
			/*��ĳ�����������Ժ��յ�g_cond�źţ�g_send!=0��*/
			pthread_cond_wait( &g_cond, &g_mutex );
		}
		//�˴���Ȼ������
		/*�����̳߳��˳�*/
		if ( g_pool_run == 0 )
		{
			//�����е����ͷ�
			pthread_mutex_unlock( &g_mutex );
			break;
		}
		/*ִ��һ��������һ����������,�����ڴ˴�����������ʱ��-1*/
		g_send --;
		pthread_mutex_unlock( &g_mutex );
		
		memset(recvbuf,0,sizeof(recvbuf));
		memset(sendbuf,0,sizeof(sendbuf));
		
        if(0!=read(connfd, recvbuf, sizeof(recvbuf)))
		{
			printf("recv msg:%s\n",recvbuf);
			 
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
			close(connfd);	
		}
       else
	   {
		   printf("No recv from client!!!\n");
		   exit(1);
	   }
		close(server_sockfd);
		usleep( 20000 );
		//printf("solved_gsend:%d\n",g_send);
	}

	return NULL;
}


void main(int argc,char *argv[]) {
    ///����sockfd
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///����sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(CALPORT_THREAD);
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

	int i, j;
	pthread_t tid[THREAD_POOL_SIZE];
	/*���ͺ�sig_handler�йص��źţ����߳��˳�*/
	signal( SIGTERM, sig_handler );

	g_run = 1;
	g_send = 0;

	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )
	{
		/*���ڴ����̹߳������յ�pthread_create���صĴ�����,�̳߳ع���ʧ��*/
		if ( pthread_create( tid + i, NULL, thr_routine, &i ) != 0 )
		{
			//����һ��͹��ˣ�ֱ�Ӿ���0
			g_run = 0;
			//��֪�������̱߳��������㲥�������������ź�
			pthread_cond_broadcast( &g_cond );
			//��ʬ�����������Ѿ��������̻߳���
			for ( j = 0; j < i; j ++ )pthread_join( tid[j], NULL );
			return;
		}
	}

	while ( g_run )usleep( 10000 );

	g_pool_run = 0;
	pthread_cond_broadcast( &g_cond );
	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )pthread_join( tid[i], NULL );

	printf( "System Will Quit ...\n" );


}

			
 

