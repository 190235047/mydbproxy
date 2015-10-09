struct myevent_s
{
	int fd;
	void (*call_back)(int fd, int events, void *arg);
	int events;
	void *arg;
	int status;              /* 1:inepollwaitlist,0notin */
	char buff[128];          /* recvdatabuffer */
	int len, s_offset;
	long last_active;        /* lastactivetime */
};
void EventSet(struct myevent_s *ev, int fd, void (*call_back)( int, int, void*), void *arg);
void EventAdd(int epollFd, int events, struct myevent_s *ev);
void EventDel(int epollFd, struct myevent_s *ev);
