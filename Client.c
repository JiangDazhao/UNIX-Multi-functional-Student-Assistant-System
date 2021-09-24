#include"header.h"
#include "set_lock.c"
#include "login.c"
#define BUFFER_SIZE 256	//读写信息缓冲区长度
#define CHATPORT 9001
void* clientthread(void* arg);

int main(int argc,char** argv){
	//提示用户输入账号和密码
    if(argc!=3){
        printf("请输入账号和密码!!!\n");
        return -1;   
    }
	
	//进行用户名昵称和密码的验证
    char* clientname =(char*)argv[1];
    char* userpsw = (char*)argv[2];
    switch (Login(clientname,userpsw)){
       case 0:printf("需要注册!!!\n");  
              regist(clientname,userpsw);
              printf("用户不存在，已经自动注册，请重新登录!!!\n");
              return -1;          
       case 1:printf("登陆成功!!!\n");            
              break;          
       case 2:printf("登录失败!!!\n"); 
              printf("用户密码错误，请重新登录!!!\n");
              return -1;           
    }
    if(sizeof(clientname)==0){
        clientname = "无名氏";
    }
	
	//设置登录与登出提示语
    char* cstr_connetc1 ="上线了!!!";
    char* cstr_close1 = "下线了!!!";
    char* clientnamecstr_connetc1 =(char *) malloc(strlen(clientname) + strlen(cstr_connetc1));
    char* clientnameClose =(char *) malloc(strlen(clientname) + strlen(cstr_close1));
    //拼接形成登录成功提示语
	strcpy(clientnamecstr_connetc1, clientname);
    strcat(clientnamecstr_connetc1, cstr_connetc1);
	//拼接形成离开聊天室提示语
    strcpy(clientnameClose, clientname);
    strcat(clientnameClose, cstr_close1);
   
    
	
//	以下进行客户端的网络连接
	//定义读写缓存区
	char buff[BUFFER_SIZE];	
    //声明客户端套接字 clientSocket1
    int clientSocket1;
    clientSocket1=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serveraddr; 
    memset(&serveraddr,0,sizeof(struct sockaddr_in));    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(CHATPORT);
    inet_pton(AF_INET,"127.0.0.1",&(serveraddr.sin_addr.s_addr));


    if(connect(clientSocket1,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
        perror("未连接到服务器\n");
        return -1;    
    }else{
        send(clientSocket1,clientnamecstr_connetc1,sizeof(buff),0);
    }
	//创建客户端的连接线程
    pthread_t tid1;
    pthread_create(&tid1,NULL,clientthread,(void*)clientSocket1);
    while(1)
	{
            memset(buff,0,BUFFER_SIZE);   
			//从键盘读入消息
            fgets(buff,BUFFER_SIZE,stdin);
            if (buff[strlen(buff)-1] == '\n'){
                buff[strlen(buff)-1] = '\0'; 
             }
			 
			//聊天结束的标志
            if(strncmp(buff,"over",4)==0){
               //send函数用于向TCP连接的另一端发送数据，最后一个参数一般为0
               send(clientSocket1,buff,BUFFER_SIZE,0);  
			    break;                         
            }
			
			//进行正常聊天的内容拼接
            char* clientname =(char*)argv[1];
            char* Message ="说: ";
            char* clientnameMessage =(char *) malloc(strlen(clientname) + strlen(Message));
            strcpy(clientnameMessage, clientname);
            strcat(clientnameMessage, Message);
            strcat(clientnameMessage, buff);
			//将消息发送给服务器
            send(clientSocket1,clientnameMessage,sizeof(buff),0);

     }
    close(clientSocket1);
    return 0;
    
}

//进行线程入口函数的定义
void* clientthread(void* arg){
    char buff[BUFFER_SIZE];
     int clientSocket1 = (int)arg;
     while(1){
        memset(buff,0,BUFFER_SIZE);
		//接受来自服务器的消息，若不为空则输出
        recv(clientSocket1,buff,BUFFER_SIZE,0);
        if(buff[0]!=0){
             printf("%s\n",buff);
        }
       
    }
}


