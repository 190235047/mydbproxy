#include <sys/epoll.h>
#include <stdio.h>
//#include "epoll.h"
#include "network.h"

#define MAX_EVENTS 100
#define PORT 	   8888;

int main(int argc, char **argv)
{
	unsigned short port = PORT; /* defaultport */
	int listen_fd;
	listen_fd = InitListenSocket(port);
	return 0;
}