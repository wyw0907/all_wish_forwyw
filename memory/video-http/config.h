#ifndef __CONFIG_H
#define __CONFIG_H

#include <syslog.h>
#include <stdio.h>

#define TCP_ADDR        ""
#define TCP_PROT        9999

#define LOG(...)        {char _buf[64]={0};snprintf(_buf,63,__VA_ARGS__);\
                        fprintf(stderr,"%s",_buf);syslog(LOG_INFO,"%s",_buf);}

struct _global{

}Global;

//start tcp server,return listen socket,
int start_server();

#endif
