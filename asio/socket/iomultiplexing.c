#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h> // for inet_addr
#include <string.h>
#include <poll.h>
#include <sys/epoll.h>

#define MAXBUF 256

void child_process(void)
{
    sleep(2);
    char msg[MAXBUF];
    struct sockaddr_in addr = {0};
    int n, sockfd, num = 1;
    srandom(getpid()); // using the pid as the seed
    
    // create socket and connect to the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    printf("child {%d} connected\n", getpid());
    while (1) {
        int sl = (random() % 10) + 1;
        num++;
        sleep(sl); // randomly sleep for [1-10 seconds]
        sprintf(msg, "Test message %d from client %d", num, getpid());
        n = write(sockfd, msg, strlen(msg));
    }
}

int main()
{
    char buffer[MAXBUF];
    struct sockaddr_in addr;
    struct sockaddr_in client;
    int addrlen, n, i, max = 0;
    int sockfd, commfd;
    fd_set rset;

    for (i = 0; i < 5; ++i)
    {
        if (fork() == 0) // 0 means it is child process
        {
            child_process();
            exit(0);
        }
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2000);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sockfd, 5);

    /* for select usage
    int fds[5];
    for (i = 0; i < 5; ++i)
    {
        memset(&client, 0, sizeof(client));
        addrlen = sizeof(client);
        fds[i] = accept(sockfd, (struct sockaddr*)&client, &addrlen);
        if (fds[i] > max) { max = fds[i]; }
    }

    while (1) {
        FD_ZERO(&rset);
        for (i = 0; i < 5; ++i) {
            FD_SET(fds[i], &rset);
        }

        puts("round again");
        // if both fields of timeval structure are zero, then 
        // select() returns immediately. This is useful for polling
        // If timeout is NULL, select blocks indefinitely waiting for a fd ready
        select(max+1, &rset, NULL, NULL, NULL);
        for (i = 0; i < 5; ++i) {
            if (FD_ISSET(fds[i], &rset)) {
                memset(buffer, 0, MAXBUF);
                read(fds[i], buffer, MAXBUF);
                puts(buffer);
            }
        }
    }
    */

    /*
    struct pollfd pollfds[5];
    for (i = 0; i < 5; ++i)
    {
        memset(&client, 0, sizeof(client));
        addrlen = sizeof(client);
        pollfds[i].fd = accept(sockfd, (struct sockaddr*)&client, &addrlen);
        pollfds[i].events = POLLIN;
    }
    sleep(1);
    while (1) {
        puts("round again");
        poll(pollfds, 5, 50000); // 0.5 second timeout
        
        for (i = 0; i < 5; ++i) {
            if (pollfds[i].revents & POLLIN) {
                pollfds[i].revents = 0;
                memset(buffer, 0, MAXBUF);
                read(pollfds[i].fd, buffer, MAXBUF);
                puts(buffer);
            }
        }
    }
    */

    struct epoll_event events[5];
    int epfd = epoll_create(10);

    for (i = 0; i < 5; ++i)
    {
        // for each new connection, we create an epoll_event object and 
        // add to the context
        static struct epoll_event ev;
        memset(&client, 0, sizeof(client));
        addrlen = sizeof(client);
        ev.data.fd = accept(sockfd, (struct sockaddr*)&client, &addrlen);
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    }

    while (1) {
        puts("round again");
        int nfds = epoll_wait(epfd, events, 5, 10000);

        for (i = 0; i < nfds; i++) {
            memset(buffer, 0, MAXBUF);
            read(events[i].data.fd, buffer, MAXBUF);
            puts(buffer);
        }
    }

    return 0;
}

// this program has a server and a bunch of clients all within itself
