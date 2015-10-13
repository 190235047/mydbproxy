#include "epoll.h"
#include <stdio.h>
#include <string.h>

int poll_create(int max_events)
{
	int fd;
	fd = epoll_create(max_events);
	if (fd <= 0) {
		return -1;
		printf("create epoll failed.%d\n", fd);
	}
	return fd;
}
/* set event */
void poll_event_set(struct myevent_s *ev, int fd, void (*call_back)(int, int, void*, int))
{
	ev->fd		    = fd;
	ev->call_back 	= call_back;
	ev->events	 	= 0;
	ev->arg			= (void *) ev;
	ev->status	 	= 0;
	bzero(ev->buff, sizeof(ev->buff));
	ev->s_offset  	= 0;
	ev->len		  	= 0;
	ev->last_active = time(NULL);
}
/* add/mod an event to epoll */
void poll_event_add(int epoll_fd, int events, void (*call_back)(int, int, void*, int), int fd)
{
	struct myevent_s *ev;
	struct epoll_event epv = {0, {0}};
	int op;
	ev = (struct myevent_s *) malloc (sizeof(struct myevent_s));  //********************相应地方添加free
	poll_event_set(ev, fd, call_back);
	epv.data.ptr = ev;
	epv.events   = ev->events = events;
	if (ev->status == 1) {
		op = EPOLL_CTL_MOD;
	} else {
		op = EPOLL_CTL_ADD;
		ev->status	= 1;
	}
	if (epoll_ctl(epoll_fd, op, ev->fd, &epv) < 0)
		printf("Event Add failed [fd=%d],evnets[%d]\n", ev->fd, events);
	else
		printf("Event Add OK [fd=%d],op=%d,evnets[%0X]\n", ev->fd, op, events);
}
void poll_event_mod(int epoll_fd, int events, void (*call_back)(int, int, void*, int), struct myevent_s *ev)
{
	//struct myevent_s *ev = (struct myevent_s *) events[i].data.ptr;
	struct epoll_event epv = {0, {0}};
	ev->call_back = call_back;
	epv.data.ptr  = ev;
	epv.events    = ev->events = events;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev->fd, &epv) < 0)
		printf("Event Modify failed [fd=%d],evnets[%d]\n", ev->fd, events);
	else
		printf("Event Modify OK [fd=%d],op=%d,evnets[%0X]\n", ev->fd, EPOLL_CTL_MOD, events);
}
/* delete an event from epoll */
void poll_event_del(int epoll_fd, struct myevent_s *ev)
{
	struct epoll_event epv = { 0, { 0 } };
	if (ev->status != 1)
		return;
	epv.data.ptr	= ev;
	ev->status	= 0;
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev->fd, &epv );
}