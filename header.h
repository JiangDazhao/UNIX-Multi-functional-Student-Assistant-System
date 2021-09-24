//��ֹ������ͻ��������Ҫ�鷳 

#ifndef _LINUX_TEST_CODE_HEADER_

#define _LINUX_TEST_CODE_HEADER_



#ifndef _GNU_SOURCE

#define _GNU_SOURCE

#endif



/* ANSI C */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

#include <stdarg.h>

#include <ctype.h>

#include <errno.h>

#include <limits.h>

#include <setjmp.h>

#include <assert.h>



/* MATH */
#include <math.h>



/* UNIX ����UNIX��*/

#include <unistd.h>

#include <fcntl.h>

#include <signal.h>

#include <dirent.h>

#include <sys/ioctl.h>

#include <sys/types.h>

#include <sys/wait.h>

#include <sys/stat.h>

#include <sys/mman.h>

#include <sys/file.h>

#include <sys/epoll.h>

#include <sys/time.h>

#include <sys/param.h>

#include <semaphore.h>

#include <sys/ipc.h>

#include <sys/msg.h>



/* BSD NETWORK ,�����̵Ĳ���������׼���������������ƺ�С*/

#include <sys/socket.h>

#include <sys/un.h>

#include <netdb.h>

#include <arpa/inet.h>

#include <netinet/in.h>

#include <netinet/tcp.h>

#include <netinet/ip.h>

#include <netinet/ip_icmp.h>

#include <net/if.h>

#include <net/if_arp.h>

#include <ifaddrs.h>




/* POSIX thread ,����ֲ�Բ���ϵͳUNIX������ϵͳ���ƣ�*/

#include <pthread.h>



#endif 
/* _LINUX_TEST_CODE_HEADER_ */

