#include "sys/types.h"
#include "sys/socket.h"
// #include "sys/stat.h"
#include "sys/epoll.h"
// #include "sys/wait.h"

#include "arpa/inet.h"
#include "fcntl.h"

#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"

int set_non_block(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
    return 0;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = 0;

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));

    if (ret != 0)
    {
        perror("bind");
        close(fd);
        return ret;
    }

    ret = listen(fd, 1024);
    if (ret != 0)
    {
        perror("listen");
        close(fd);
        return ret;
    }

    set_non_block(fd);

    int epfd = epoll_create(1024);
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if (ret != 0)
    {
        perror("epoll_ctl");
        close(fd);
        return 0;
    }

    while (1)
    {
        struct epoll_event ev[8];
        ret = epoll_wait(epfd, ev, 8, 105000); // blocks
        if (ret != 0)
        {
            if (errno == EINTR) { continue; }
        }

        for (int i = 0; i < ret; ++i)
        {
            int newfd;
            if (ev[i].data.fd == fd)
            {
                // socket fd
                newfd = accept(ev[i].data.fd, NULL, NULL);
                event.data.fd = newfd;
                event.events = EPOLLIN;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &event);
                if (ret != 0)
                {
                    perror("epoll_ctl");
                    close(newfd);
                    close(fd);
                    return -1;
                }
                printf("added the new fd:%d\n", newfd);
            }
            else
            {
                int connectfd = ev[i].data.fd;
                char buf[1024] = {0};
                if (read(connectfd, buf, sizeof(buf)) > 0)
                {
                    printf("recv buf:%s\n", buf);
                }
                else
                {
                    printf("client connection closed\n");
                    close(connectfd);
                }
            }
        }
    }
    return 0;
}
