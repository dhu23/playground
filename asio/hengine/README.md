# Architecture

This is to develop a simplified backbone of a tradings sytem, which a 
messaging bus layer in the middle and market data producer and strategy that
comsumes data. The main purpose of this is to gain experience dealing with
asynchronous I/O handling and the end result provides a nice framework to 
collect latency profiles. It also provides a platform to build flexible and 
reusable application layer components.

The center piece of messaging bus shall be done in C++ for learning reasons.
There should be multiple ways of configuring this center piece so that 
different techniques can be compared with each other. These techniques include
thread-based concurrency, I/O multiplexing based solutions that feature select, 
poll, or epoll as the barebone. 

The data producers and downstream data comsumers can be written in any 
languages of interest, including C, C++, Python, Haskell. 
