#include <core.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LED 	   2
#define SIG_INPUT  3
#define SIG_OUTPUT 4

#define NET_PORT   9999
#define LED1_HOST  ""  
#define LED2_HOST  ""
#define LED3_HOST  ""
#define LED4_HOST  ""

#define THIS_HOST  LED1_HOST     
#define NEXT_HOST  LED2_HOST
int sockfd;
char recvbuf[BUFSIZ];

void init_udp_socket();

void setup()
{
	pinMode(LED,OUTPUT);
	init_udp_socket();
}

void loop()
{
}

void init_udp_socket()
{
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0){	
		printf("socket error!\n");
		return ;
	}

	struct sockaddr_in thisaddr;
	thisaddr.sin_family = AF_INET;
	thisaddr.sin_port = htons(NET_PORT);
	thisaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	struct sockaddr_in nextaddr;
	thisaddr.sin_family = AF_INET;
	thisaddr.sin_port = htons(NET_PORT);
	inet_pton(AF_INET,NEXT_HOST,&thisaddr.sin_addr);

	bind(sockfd,(struct sockaddr *)&thisaddr,sizeof(thisaddr));

	while(recvfrom(sockfd,recvbuf,BUFSIZ,0,NULL,NULL))
	{
		digitalWrite(LED,HIGH);
		delay(1000);
		digitalWrite(LED,LOW);
		sendto(sockfd,recvbuf,BUFSIZ,0,(struct sockaddr *)&nextaddr,sizeof(nextaddr));
	}
}




