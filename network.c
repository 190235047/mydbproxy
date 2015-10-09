#include "network.h"
int InitListenSocket(short port)
{
	int listenFd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(listenFd, F_SETFL, O_NONBLOCK ); /* setnon-blocking */
	printf("server listen fd=%d\n", listenFd);
/* bind&listen */
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family		= AF_INET;
	sin.sin_addr.s_addr	= INADDR_ANY;
	sin.sin_port		= htons(port);
	bind(listenFd, (struct sockaddr*)&sin, sizeof(sin));
	//listen(listenFd, 5 );
	if (listen(listenFd,5) < 0){  
   		 printf("listen error\n");
   	     //exit(1);  
	} 
	return listenFd;
}