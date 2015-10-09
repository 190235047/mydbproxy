/* setevent */
void EventSet(struct myevent_s *ev, int fd, void (*call_back)( int, int, void*), void *arg)
{
	ev->fd		= fd;
	ev->call_back	= call_back;
	ev->events	= 0;
	ev->arg		= arg;
	ev->status	= 0;
	bzero( ev->buff, sizeof(ev->buff) );
	ev->s_offset	= 0;
	ev->len		= 0;
	ev->last_active = time( NULL );
}
/* add/modaneventtoepoll */
void EventAdd(int epollFd, int events, struct myevent_s *ev)
{
	struct epoll_event epv = { 0, { 0 } };
	int op;
	epv.data.ptr = ev;
	epv.events   = ev->events = events;
	if ( ev->status == 1 )
	{
		op = EPOLL_CTL_MOD;
	}else  {
		op		= EPOLL_CTL_ADD;
		ev->status	= 1;
	}
	if ( epoll_ctl( epollFd, op, ev->fd, &epv ) < 0 )
		printf( "EventAddfailed[fd=%d],evnets[%d]\n", ev->fd, events );
	else
		printf( "EventAddOK[fd=%d],op=%d,evnets[%0X]\n", ev->fd, op, events );
}
/* deleteaneventfromepoll */
void EventDel(int epollFd, struct myevent_s *ev)
{
	struct epoll_event epv = { 0, { 0 } };
	if ( ev->status != 1 )
		return;
	epv.data.ptr	= ev;
	ev->status	= 0;
	epoll_ctl( epollFd, EPOLL_CTL_DEL, ev->fd, &epv );
}