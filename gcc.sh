gcc  -o test dbproxy.c epoll.c network.c -I ./mysql/include/ -L ./mysql/lib/  -lmysqlclient
