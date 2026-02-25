mini_serv

A minimalist TCP chat server written in C

Overview

Multiple clients connect to the server and can exchange messages with each other.

The server handles all routing, identifies each client with a unique ID, and notifies others when someone joins or leaves.

Build & Run

bashgcc -Wall -Wextra -o mini_serv mini_serv.c

./mini_serv <port>

Test it with nc in separate terminals:

bashnc 127.0.0.1 <port>

How it works


Built around select() for multiplexed, non-blocking I/O

Buffers incomplete messages until a newline is received

Broadcasts events (join/leave/messages) to all other connected clients

No threads, no external libraries — pure C standard + POSIX sockets


Notes

Only binds to 127.0.0.1

Clients are identified by an auto-incremented integer ID starting at 0
