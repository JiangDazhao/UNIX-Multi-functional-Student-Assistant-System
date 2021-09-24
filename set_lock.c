#include"header.h"
//进行文件锁的设置，保证消息记录的准确性
int set_lock(int fd,int type){
	struct flock lock;
	lock.l_whence = SEEK_SET;//当前文件偏移量从头开始
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_type = type;
	lock.l_pid = -1;

	//设置记录锁
	fcntl(fd, F_GETLK, &lock);
	/**判断文件是否能够上锁*/
	if (lock.l_type != F_UNLCK)
	{
       		 /* 判断文件不能上锁的原因 */
 		if (lock.l_type == F_RDLCK) /* 该文件已有读取锁 */	  
       		{
			printf("读取锁已被线程 %d 部署 \n", lock.l_pid);
		}else if (lock.l_type == F_WRLCK)/**该文件已有写入锁*/ 
		{
			printf("写入锁已被线程 %d 部署\n", lock.l_pid);
		}					
	}
	lock.l_type = type;
    /**设置阻塞锁*/
	if ((fcntl(fd, F_SETLKW, &lock)) < 0)
	{
		printf("上锁失败:类型为 = %d\n", lock.l_type);
		return 1;
	}	
	return 0;
}
