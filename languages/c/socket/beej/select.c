// select.c -- a select() demo
//
// FD_SET(int fd, fd_set *set); // add fd to the set
// FD_CLR(int fd, fd_set *set); // remove fd from the set
// FD_ISSET(int fd, fd_set *set); // return true if fd is in the set
// FD_ZERO(fd_set *set); // clear all entries from the set
//
// int select(
//     int numfds, fd_set *readfds, fd_set *writefds, 
//     fd_set *exceptfds, struct timeval *timeout);
//
// struct timeval {
//     int tv_sec; // seconds
//     int tv_usec; // microseconds
// };
//
// set to 0 -- timeout immediately
// set to NULL -- never timeout, wait until the first fd is ready
// set a certain set to NULL if you don't care about waiting

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0 // file descriptor for standard input

int main(void)
{
    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 2;
    tv.tv_usec = 500000;

    FD_ZERO(&readfds);
    FD_SET(STDIN, &readfds);

    printf("before running select\n");

    // don't care about writefds and exceptfds
    select(STDIN+1, &readfds, NULL, NULL, &tv);

    if (FD_ISSET(STDIN, &readfds)) {
        printf("A key was pressed!\n");
    } else {
        printf("Timed out.\n");
    }

    return 0;
}

// one more not of interest about select: if you have a socket that is 
// listening then you can check to see if there is a new connection by
// putting that socket file descriptor in the readfds set.
