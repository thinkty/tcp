#include "server.h"

int main(int argc, char *argv[])
{
	args_t args;
	if (check_args(argc, argv, &args)) {
		return ERR;
	}

	/* Setup TCP IPv4 socket */
	int sock;
	if ((sock = tcp_listen(args.port)) == ERR) {
		eprintf("Failed to listen");
		return ERR;
	}

	printf("port: %u, ", args.port);
	switch (args.mode) {
	case MODE_TPOOL:
		printf("mode: threadpool (%u)\n", args.pool_size);
		start_tpool(sock, args.pool_size);
		break;
	case MODE_THREAD:
		printf("mode: thread\n");
		start_thread(sock);
		break;
	case MODE_CHILDP:
		printf("mode: child process\n");
		start_childp(sock);
		break;
	default:
		eprintf("Unrecognized handler option");
		close(sock);
		return ERR;
	}

	/* socket is closed in the handlers */
	return OK;
}

void start_tpool(int sock, uint32_t pool_size)
{
	pthread_t * pool = calloc(pool_size, sizeof(pthread_t));
	if (pool == NULL) {
		close(sock);
		eprintf("Failed to allocate memory for pool");
		return;
	}

	/* Create thread pool and handle connections */
	for (uint32_t i = 0; i < pool_size; i++) {
		if ((errno = pthread_create(&pool[i], NULL, accept_task, &sock)) != 0) {
			perror("pthread_create()");
			close(sock);
			free(pool);
			return;
		}
	}

	// TODO: is there a way to signal the threads to close?

	for (uint32_t i = 0; i < pool_size; i++) {
		if ((errno = pthread_join(pool[i], NULL)) != 0) {
			perror("pthread_join()");
			close(sock);
			free(pool);
			return;
		}
	}

	/* Clean-up */
	close(sock);
	free(pool);
}

void * accept_task(void * sock)
{
	for (;;) {
		int csock;
		if (tcp_accept(*((int *) sock), &csock)) {
			eprintf("Failed to accept new connection");
			return NULL;
		}

		echo(csock);
	}

	return NULL;
}

void start_thread(int sock)
{
	for (;;) {
		int * csock = malloc(sizeof(int));
		if (csock == NULL) {
			eprintf("Failed to allocate client socket");
			close(sock);
			break;
		}

		if (tcp_accept(sock, csock) != OK) {
			eprintf("Failed to accept new connection");
			break;
		}

		/* Create new thread and detach on new connection */
		pthread_t thread;
		if ((errno = pthread_create(&thread, NULL, echo_task, csock))) {
			perror("pthread_create(handler_thread)");
			break;
		}
		if ((errno = pthread_detach(thread))) {
			perror("pthread_detach(handler_thread)");
			break;
		}
	}

	close(sock);
}

void * echo_task(void * csock)
{
	echo(*((int *) csock));

	/* Clean up */
	free((int *) csock);
	return NULL;
}

void start_childp(int sock)
{
	/* Reap child automatically */
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("signal()");
		return;
	}

	for (;;) {
		int * csock = malloc(sizeof(int));
		if (csock == NULL) {
			eprintf("Failed to allocate client socket");
			close(sock);
			break;
		}

		if (tcp_accept(sock, csock) != OK) {
			eprintf("Failed to accept new connection");
			break;
		}

		/* Create new child process */
		pid_t pid = fork();
		switch(pid) {
		case -1:
			perror("fork()");
			break;
		case 0:
			echo(*csock);
			free(csock);
			close(sock);
			return;
		default:
			printf("Created new proc [%jd]\n", (intmax_t) pid);
			close(*csock);
			free(csock);
			break;
		}
	}

	close(sock);
}

void echo(int csock)
{
	char buff[SOCK_BUFFER_SIZE] = {0};
	ssize_t ret;

	for (;;) {

		/* Read from client socket */
		if ((ret = recv(csock, buff, SOCK_BUFFER_SIZE, 0)) < 0) {
			perror("recv()");
			break;
		}

		/* EOF */
		if (ret == 0) {
			break;
		}
		buff[ret] = '\0';

		/* Echo */
		if (send(csock, buff, (unsigned) ret, 0) < 0) {
			perror("send()");
			break;
		}
	}

	/* Clean up */
	close(csock);
}

