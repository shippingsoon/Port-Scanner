#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "c-scan.h"

//Attempts a non-blocking connection to a given host:port.
int scanner(const char * host, int port, int timeout)
{
	//This socket address struct contains useful information for our connection.
	struct sockaddr_in address;
	//This struct will determine when our connection times out.
	struct timeval tv;
	fd_set write_fds;
	socklen_t so_error_len;
	//The socket descriptor and error status. 
	int sd, so_error;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(host); 
	address.sin_port = htons(port);
	//Seconds to timeout.
	tv.tv_sec = timeout;
	//Microseconds to timeout.
	tv.tv_usec = timeout * 1000000;
	FD_ZERO(&write_fds);
	so_error_len = sizeof(so_error);
	//Retrieve a socket descriptor.
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return terminate("socket() An error has occurred", 0);
	//Make our socket non-blocking.
	fcntl(sd, F_SETFL, O_NONBLOCK);
	//The connect() function always returns -1 when the non-blocking flag is set on the socket.
	if (connect(sd, (struct sockaddr *) &address, sizeof(address)) == -1) {
		switch (errno) {
			case EWOULDBLOCK:
			case EINPROGRESS:
				break;
			default:
				return terminate("connect() An error has occurred", sd);
		}
	}
	FD_SET(sd, &write_fds);
	if (select(sd + 1, NULL, &write_fds, NULL, &tv) == -1)
		return terminate("select() An error has occurred", sd);
	if (getsockopt(sd, SOL_SOCKET, SO_ERROR, &so_error, &so_error_len) < 0)
		return terminate("getsockopt() An error has occurred", sd);
	//Close the socket.
	close(sd);
	return (so_error == 0);
}

//The thread's task.
void *task(void *thread_args)
{
	struct thread_addr *args;
	int status;
	
	args = thread_args;
	status = scanner(args->host, args->port, args->timeout);
	printf("Thread #%i\t%s:%4.i is %s\n", args->thread_id, args->host, args->port, ((status) ? "active" : "inactive"));
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[MAX_PORTS];
	struct thread_addr args[MAX_PORTS];
	int thread_id, timeout, opt;
	char host[INET_ADDRSTRLEN];
	unsigned int ports[] = {
		7,
		18,
		20,
		21,
		22,
		23,
		25,
		29,
		37,
		42,
		43,
		49,
		53,
		69,
		70,
		79,
		80,
		103,
		108,
		109,
		110,
		115,
		118,
		119,
		137,
		139,
		143,
		150,
		156,
		161,
		179,
		190,
		194,
		197,
		389,
		396,
		443,
		444,
		445,
		458,
		546,
		547,
		563,
		569,
		1080
	};
	
	//Set a placeholder host.
	strncpy(host, "127.0.0.1", (size_t) INET_ADDRSTRLEN);
	//The time in seconds that our connection will wait before timing out.
	timeout = 3;
	//Use getopt to parse command line arguments.
	while ((opt = getopt(argc, argv, "h:t:v")) != -1) {
		switch (opt) {
			case 'v':
				printf("%s\n", VERSION);
				return EXIT_SUCCESS;
			case 'h':
				strncpy(host, optarg, (size_t) INET_ADDRSTRLEN);
				break;
			case 't':
				timeout = atoi(optarg);
				break;
			default:
				fprintf(stderr, "\nUsage: %s [-h host] [-t timeout] [-v version]\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	//Loop through the ports.
	for (thread_id = 0; thread_id < MAX_PORTS; thread_id++) {
		//Set the host, port, connection timeout and thread ID.
		strncpy(args[thread_id].host, host, (size_t) INET_ADDRSTRLEN);
		args[thread_id].port = ports[thread_id];
		args[thread_id].timeout = timeout;
		args[thread_id].thread_id = thread_id;
		//Create the task.
		if (pthread_create(&threads[thread_id], NULL, task, (void *) &args[thread_id])) {
			#ifdef DEBUG_MODE
			perror("pthread_create() error");
			#endif
			return EXIT_FAILURE;
		};
	}
	pthread_exit(NULL);
}

//Close the socket and print useful debugging information.
int terminate(const char *s, int sock)
{
	#ifdef DEBUG_MODE
	perror(s);
	#endif
	if (sock)
		close(sock);
	return 0;
}

/* Copyright 2014 Shipping Soon */