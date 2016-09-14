#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config.h"
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

int start_server()
{
    int rst;
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0){
        LOG("socket error\n");
        return -1;
    }
    int on = 0;
    rst = setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(rst < 0){
        LOG("setsockopt error\n");
        return -1;
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(TCP_PROT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    rst = bind(fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(rst < 0){
        LOG("bind error\n");
        return -1;
    }

    rst = listen(fd,5);
    if(rst < 0){
        LOG("listen error!\n");
        return -1;
    }
    printf("listen\n");
    return fd;
}


int main(void)
{
    int listenfd = start_server();
    if(listenfd < 0){
        LOG("start_server error!\n");
        return;
    }

    pthread_t srv_tid;


    printf("hello world!\n");
    return 0;
}

