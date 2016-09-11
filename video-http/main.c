#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config.h"
#include <string.h>
#include <fcntl.h>
int start_server()
{
    int rst;
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0){
        LOG("socket error\n");
        return;
    }
    int on = 0;
    rst = setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(rst < 0){
        LOG("setsockopt error\n");
        return;
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(TCP_PROT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    rst = bind(fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(rst < 0){
        LOG("bind error\n");
        return;
    }


    return fd;
}


int main(void)
{
    int listenfd = start_server();
    printf("hello world!\n");
    return 0;
}

