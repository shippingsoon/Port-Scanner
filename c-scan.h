#define MAX_PORTS 45
#define DEBUG_MODE
#define VERSION "c-scan v0.02"
#define ISSET(x, y) ((x & (y)) == (y))
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

//This struct will hold the data we pass to the threads.
struct thread_addr {
	char host[INET_ADDRSTRLEN];
	int port, timeout, thread_id;
};

//Close the socket and print useful debugging information.
int terminate(const char *s, int sock);

//The thread's task.
void *task(void *thread_args);

//Attempts a non-blocking connection to a given host:port.
int scanner(const char * host, int port, int timeout);

/* Copyright 2014 Shipping Soon */