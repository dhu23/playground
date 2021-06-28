#include <sys/types.h>
#include <sys/socket.h>
// #include <sys/stat.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sig_handle(int sig)
{
    printf("recv signal:%d\n", sig);
}

int main(int argc, char* argv[])
{
    signal(SIGPIPE, sig_handle);
    if (argc < 2)
    {
        printf("usage:%s + [count]\n", argv[0]);
        return 0;
    }

    int count = atoi(argv[1]);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        return 0;
    }

    listen(fd, 250);
    int is_child_process = 0; 
    // determine which process is in, parent process 0, child process 1
    for (int i = 0; i < count; i++)
    {
        pid_t pid = fork();
        if (pid == 0) // for child process
        {
            is_child_process = 1;
            break;
        }
    }

    struct epoll_event ev;
    ev.events = EPOLLIN|EPOLLET; // edge trigger mode
    ev.data.fd = fd;
    int epfd = epoll_create(1024);
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);

    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    while (1)
    {
        struct epoll_event evs[10];
        int process_count = epoll_wait(epfd, evs, 10, 5000);
        if (process_count == 0) { continue; }

        for (int i = 0; i < process_count; i++)
        {
            if (evs[i].data.fd == fd)
            {
                int ret = accept(evs[i].data.fd, NULL, NULL);
                if (ret == -1)
                {
                    printf("errno:%s\n", strerror(errno));
                    break;
                }

                ev.data.fd = ret;
                epoll_ctl(epfd, EPOLL_CTL_ADD, ret, &ev);
            }
            else
            {
                char buf[1024];

                int ret = read(evs[i].data.fd, buf, sizeof(buf));
                if (ret == -1)
                {
                    perror("read");
                    if (errno == EINTR) { break; }
                    exit(0);
                }
                else if (ret == 0)
                {
                    // normal exit
                    close(evs[i].data.fd);
                    break;
                }
            }
        }
    }
    if (!is_child_process)
    {
        for (int i = 0; i < count; i++)
        {
            wait(NULL); // wait until all child processes exit
        }
    }
    return 0;
}
