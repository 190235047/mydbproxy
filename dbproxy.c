#include <stdio.h>
#include "epoll.h"
#include "network.h"

#define MAX_EVENTS 100
#define PORT 	   8888;
#define PROCESS_NUM 1;

int main(int argc, char **argv)
{
	unsigned short port = PORT; /* defaultport */
	int listen_socket, process_num, i;
	pid_t pid;
	process_num = PROCESS_NUM;
	listen_socket = init_listen_socket(port);
	for (i = 0; i < process_num; i++){
		pid = child_make(listen_socket);
	}
	while(1){
		//printf("I am father\n");
	}
	printf("main end\n");
	return 0;
}

pid_t child_make(int listen_socket)
{
   pid_t pid;
   int poll_fd, i;
   struct epoll_event events[MAX_EVENTS];
   pid = fork();
   if (0 < pid) {
   	   //printf("fork error, pid = %d\n", pid);
   	   printf("I am father\n");
       return pid;
   }
   if (0 > pid) {
       printf("fork error, pid = %d\n", pid);
       return pid;
    }
    printf("child_make start\n");
    poll_fd = poll_create(MAX_EVENTS);
    printf("child_make poll_create\n");
    if (poll_fd == -1) {
    	printf("poll_create error\n");
    	return -1;
    }
    printf("child_make poll_add\n");

    poll_event_add(poll_fd, EPOLLIN, accept_conn, listen_socket);
    printf("child_make into while\n");
	while (1) {
		/* a simple time out check here,every time 100,better to use a mini-heap,and add time revent */
		long now = time(NULL);
		// for (i = 0; i <100; i++, checkPos++ )      /* doesn't check listen fd */
		// {
		// 	if (checkPos == MAX_EVENTS)
		// 		checkPos = 0;                   /* recycle */
		// 	if (g_Events[checkPos].status != 1 )
		// 		continue;
		// 	long duration = now - g_Events[checkPos].last_active;
		// 	if ( duration >= 60 )                   /* 60s timeout */
		// 	{
		// 		close(g_Events[checkPos].fd );
		// 		printf("[fd=%d]timeout[%d--%d].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active, now);
		// 		EventDel(g_epollFd, &g_Events[checkPos]);
		// 	}
		// }
	    /* wait for events to happen */

	    printf("epoll_wait\n");
		int fds = epoll_wait(poll_fd, events, MAX_EVENTS, 1000);
		if (fds < 0) {
			printf("epoll_wait error,exit\n" );
			break;
		}
		printf("fds : %d\n", fds);
		for (i = 0; i < fds; i++) {
			struct myevent_s *ev = (struct myevent_s *) events[i].data.ptr;
			if ( (events[i].events & EPOLLIN) && (ev->events & EPOLLIN)) {   /* read event */
				ev->call_back(ev->fd, events[i].events, ev->arg, poll_fd);
			}
			if ( (events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT)) { /* write event */
				ev->call_back(ev->fd, events[i].events, ev->arg, poll_fd);
			}
		}
	}    
	printf("child_make end\n");
    return -1;
}