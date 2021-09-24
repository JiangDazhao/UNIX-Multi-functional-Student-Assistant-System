#include"header.h"
#define BUFFER_SIZE 256	//读写信息缓冲区长度
#define USER_LIMIT 10	//存储的用户登录信息上限

//用于管理用户的登录查询 
void split( char **arr, char *str, const char *del)//字符分割函数,以del分割str存储在arr											
{												   //
    char *s =NULL;
    s=strtok(str,del);
    while(s != NULL)
    {
    *arr++ = s;
    s = strtok(NULL,del);
    }
}
//从Login.txt里获取某个用户的密码，若找不到该用户则返回无该用户
char* Map_get(char* username){
    int fd1;
    char loginBuff[BUFFER_SIZE];//用于存储从login.txt里读出来的信息 
    fd1 = open("login.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    if(fd1<0){
           printf("打开文件失败\n");			
           exit(1);
    }
    read(fd1,loginBuff,BUFFER_SIZE);
	
	//开一个字符数组存储用户信息，上限为USER_LIMIT
    char *userMsg[USER_LIMIT];
	//用于存储某个用户的用户名和密码
    char *userinfo[2];
    memset(userMsg, 0x0, sizeof(userMsg));
	//将login.txt里的用户信息进行逐行分割存入userMsg字符数组中
    split(userMsg, loginBuff, "\n"); 	
    for(int j = 0 ; j < sizeof(userMsg) ; j++){      
			//将userMsg字符数组中的逐条用户名和密码分割分别存入userinfo[0]和userinfo[1]
            split(userinfo, userMsg[j], "-");
            if(strcmp(username,userinfo[0])==0){//找到某个用户的密码
                    close(fd1);
                    return userinfo[1];//返回pwdvalue
            }else{
                if(j==USER_LIMIT-1){
                    return "没有该用户!!!\n"; 
                }
           }
    }      
}
//在Login.txt里登记好登录信息
void map_set(char* username,char* pwdvalue){
    int fd1;
    fd1 = open("login.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    if(fd1<0){
       printf("打开map文件失败\n");			
       exit(1);
    }  
    char* username1 =(char *) malloc(strlen(username) + 2);
    char* pwdvalue1 = (char *) malloc(strlen(pwdvalue) + 2);     
    strcpy(username1, username);//复制username——》username1
    strcpy(pwdvalue1, pwdvalue);
    strcat(username1,"-");  
    strcat(pwdvalue1, "\n");
	
	//将登录信息的账号密码写入Login.txt的文件操作符fd1里面
    char* userMessage =(char *) malloc(strlen(username1) + strlen(pwdvalue1)); 
    strcpy(userMessage, username1);
    strcat(userMessage, pwdvalue1);
    if(write(fd1,userMessage,strlen(userMessage))<0){
            printf("用户账号密码保存失败!!!");
        }else{
            printf("用户账号密码保存成功!!!");
        }
    close(fd1);   
}

//输入值pwdvalue为int
void Map_set(char* username,int pwdvalue){
    char buff[BUFFER_SIZE];
    snprintf(buff, sizeof(buff), "%d", pwdvalue);
    map_set(username,buff);
}

//删除用户登录信息表
void Map_delete(){
    remove("login.txt");    
}

int Login(char* username,char* pwdvalue){
    char* psw = Map_get(username);
    if(strcmp(psw,"没有该用户!!!\n")==0){
        return 0;//0 需要注册
    }else if(strcmp(psw,pwdvalue)==0){
        return 1;//1 登陆成功
    }else{
        return 2;//2 登录失败        
    }
    
}
//进行用户的自动注册
void regist(char* username,char* pwdvalue){
    map_set(username,pwdvalue);
}
// int main(int argc,char** argv){
//     char* clientname =(char*)argv[1];
//     char* userpsw = (char*)argv[2];
//     switch (login(clientname,userpsw)){
//        case 0:printf("需要注册");  
//               regist(clientname,userpsw);
//               printf("用户不存在自动注册，请重新登录！");
//               return -1;          
//        case 1:printf("登陆成功");            
//               break;          
//        case 2:printf("登录失败"); 
//               printf("用户密码错误，请重新登录！");
//               return -1;           
//     }
// }









