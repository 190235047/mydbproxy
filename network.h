#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h> //??

int init_listen_socket(short port);
void accept_conn(int fd, int events, void *arg, int poll_fd);
void recv_data(int fd, int events, void *arg, int poll_fd);
void send_data(int fd, int events, void *arg, int poll_fd);

