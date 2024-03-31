#include "server.h"

int main(int argc, char *argv[])
{
	uint16_t port;
	uint32_t pool_size;

	if (check_args(argc, argv, &port, &pool_size) == ERR) {
		eprintf("Invalid arguments");
	}

	/* Setup TCP IPv4 socket */
	int sock;
	if ((sock = tcp_listen(port)) == ERR) {
		eprintf("Failed to listen");
		return ERR;
	}

	pthread_t * threads = calloc(pool_size, sizeof(pthread_t));
	if (threads == NULL) {
		close(sock);
		eprintf("Failed to create pool");
		return ERR;
	}

	/* Create thread pool and handle connections */
	for (uint32_t i = 0; i < pool_size; i++) {
		if (pthread_create(&threads[i], NULL, server_accept, (void *) &sock)) {
			perror("pthread_create()");
			close(sock);
			free(threads);
			return ERR;
		}
	}

	for (uint32_t i = 0; i < pool_size; i++) {
		if (pthread_join(threads[i], NULL)) {
			perror("pthread_join()");
			close(sock);
			free(threads);
			return ERR;
		}
	}

	/* Clean-up */
	close(sock);
	free(threads);
	return 0;
}

int check_args(int argc, char * argv[], uint16_t * port, uint32_t * pool_size)
{
	/* Check arguments count */
	if (argc != 3) {
		eprintf("Usage: %s <port> <thread pool size>", argv[0]);
		return ERR;
	}

	/* Check port number */
	*port = (uint16_t) atoi(argv[1]);
	if (*port == 0) {
		eprintf("Failed to parse port number");
		return ERR;
	}

	/* Check thread pool size */
	*pool_size = (uint32_t) atoi(argv[2]);
	if (*pool_size == 0) {
		eprintf("Failed to parse thread pool size");
		return ERR;
	}

	return OK;
}

void * server_accept(void * sock)
{
	int csock;

	for (;;) {
		if (tcp_accept(*((int *) sock), &csock) != OK) {
			eprintf("Failed to accept new connection");
			return NULL;
		}

		handle(csock);
	}

	return NULL;
}

void handle(int csock)
{
	char buff[SOCK_BUFFER_SIZE] = {0};
	ssize_t ret;

	for (;;) {

		/* Read from client socket */
		if ((ret = recv(csock, buff, SOCK_BUFFER_SIZE, 0)) < 0) {
			perror("recv()");
			close(csock);
			return;
		}

		/* EOF */
		if (ret == 0) {
			printf("EOF\n");
			break;
		}

		buff[ret] = '\0';
		printf("< %s", buff);

		/* Echo */
		if (send(csock, buff, (unsigned) ret, 0) < 0) {
			perror("send()");
			close(csock);
			return;
		}
	}

	/* Clean up */
	close(csock);
}

