all: epoll_example

epoll_example: epoll_example.c
	gcc -Wall -Werror -o $@.tsk epoll_example.c

epoll_horizontal_trigger: epoll_horizontal_trigger.c
	gcc -Wall -Werror -o $@.tsk epoll_horizontal_trigger.c

epoll_edge_trigger: epoll_edge_trigger.c
	gcc -Wall -Werror -o $@.tsk epoll_edge_trigger.c

clean:
	@rm -v epoll_example.tsk
	@rm -v epoll_horizontal_trigger.tsk
	@rm -v epoll_edge_trigger.tsk
