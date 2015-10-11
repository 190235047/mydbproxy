#include <sys/epoll.h>
#include <malloc.h>
struct myevent_s
{
	int fd;
	void (*call_back)(int fd, int events, void *arg, int poll_fd);
	int events;
	void *arg;
	int status;              /* 1:in epoll wait list,0 not in */
	char buff[128];          /* recv data buffer */
	int len, s_offset;
	long last_active;        /* last active time */
};
int  poll_create(int max_events);
void poll_event_set(struct myevent_s *ev, int fd, void (*call_back)(int, int, void*, int));
void poll_event_add(int epoll_fd, int events, void (*call_back)(int, int, void*, int), int fd);
void poll_event_del(int epollFd, struct myevent_s *ev);
