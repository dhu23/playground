#include <stdio.h>
#include <poll.h>

int main(void)
{
    struct pollfd pfds[1]; // More if you want to monitor more

    pfds[0].fd = 0; // standard input
    pfds[0].events = POLLIN; // tell me when ready to read

    // if you needed to monitor other things, as well:
    // pfds[1].fd = some_socket; // some socket descriptor
    // pfds[1].events = POLLIN; // tell me when ready to read
    
    printf("Hit Return or wait 2.5 seconds for timeout\n");

    int num_events = poll(pfds, 1, 2500); // 2.5 second timeout
    // notice again that poll() returns the number of elements in the pfds
    // array for which events have occurred. It doesn't tell you which 
    // element in the array (you still have to scan for that), but it does
    // tell you how many entries have a non-zero revents field(so you can 
    // stop scanning after you find that many). 
    //
    // a couple questions might come up here: how to add new file 
    // descriptors to the set I pass to poll()? For this, simply make sure
    // you have enough space in the array for all you need, or realloc()
    // more spaces as needed. 
    //
    // What about deleting items from the set? For this, you can copy the 
    // last element in the array over-top the one you're deleting. And 
    // then pass in one fewer as the count to poll(). Another option is 
    // that you can set any fd field to a negative number and poll() will
    // ignore it. 

    if (num_events == 0) {
        printf("Poll timed out!\n");
    } else {
        int pollin_happened = pfds[0].revents & POLLIN;

        if (pollin_happened) {
            printf("File descriptor %d is ready to read\n", pfds[0].fd);
        } else {
            printf("Unexpected event occurred: %d\n", pfds[0].revents);
        }
    }

    return 0;
}
