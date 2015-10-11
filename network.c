#include "network.h"
#include "epoll.h"
int init_listen_socket(short port)
{
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(listen_socket, F_SETFL, O_NONBLOCK ); /* setnon-blocking */
	printf("server listen fd=%d\n", listen_socket);
	/* bind&listen */
	struct sockaddr_in sin;
	bzero(&sin, sizeof(sin));
	sin.sin_family		= AF_INET;
	sin.sin_addr.s_addr	= INADDR_ANY;
	sin.sin_port		= htons(port);
	bind(listen_socket, (struct sockaddr*)&sin, sizeof(sin));
	//listen(listenFd, 5 );
	if (listen(listen_socket,5) < 0){  
   		 printf("listen error\n");
   	     //exit(1);  
	} 
	return listen_socket;
}
void accept_conn(int fd, int events, void *arg, int poll_fd)
{
	struct sockaddr_in sin;
	socklen_t len = sizeof(struct sockaddr_in);
	int nfd, i;
	/* accept */
	if ((nfd = accept(fd, (struct sockaddr*)&sin, &len)) == -1 )
	{
		if ( errno != EAGAIN && errno != EINTR )
		{
		}
		printf( "%s:accept,%d", __func__, errno );
		return;
	}
	/* set nonblocking */
	int iret = 0;
	if ((iret = fcntl(nfd, F_SETFL, O_NONBLOCK ) ) < 0 )
	{
		printf( "%s:fcntl non blocking failed:%d", __func__, iret );
		return;
	}
	/* add a read event for receive data */
	poll_event_add(poll_fd, EPOLLIN, recv_data, nfd);	
	printf("new conn[%s:%d],pos[%d]\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), i);
}

/* receive data */
void recv_data(int fd, int events, void *arg, int poll_fd)
{
	printf("recv_data\n");
	struct myevent_s *ev = (struct myevent_s *) arg;
	int len;
    /* receive data */
	len = recv(fd, ev->buff + ev->len, sizeof(ev->buff) - 1 - ev->len, 0);
	poll_event_del(poll_fd, ev);
	if (len > 0) {
		ev->len		 += len;
		ev->buff[len] = '\0';
		printf("C[%d]:%s\n", fd, ev->buff);
    	/* change to send event */
		poll_event_add(poll_fd, EPOLLOUT, send_data, fd);
	} else if(len == 0 ) {
		close(ev->fd);
		printf("[fd=%d],closed grace fully.\n", fd);
	} else {
		close(ev->fd);
		printf("recv[fd=%d]error[%d]:%s\n", fd, errno, strerror(errno));
	}
}

/* send data */
void send_data(int fd, int events, void *arg, int poll_fd)
{
	struct myevent_s *ev = (struct myevent_s *) arg;
	int len;
	/* senddata */
	len = send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0 );
	if ( len > 0 )
	{
		printf( "send[fd=%d],[%d<->%d]%s\n", fd, len, ev->len, ev->buff );
		ev->s_offset += len;
		if (ev->s_offset == ev->len ) {
			/* change to receive event */
			poll_event_del(poll_fd, ev);
			poll_event_add(poll_fd, EPOLLIN, recv_data, fd);	
		}
	} else {
		close(ev->fd);
		poll_event_del(poll_fd, ev);
		printf("send[fd=%d]error[%d]\n", fd, errno);
	}
}
