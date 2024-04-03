#ifndef __TCP_H
#define __TCP_H

#include "common.h"

#include <arpa/inet.h>  /* sockaddr_in, htons(), htonl() */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h> /* socket(), bind(), listen(), accept() */
#include <unistd.h>     /* read(), write() */

#define SOCK_LISTEN_Q_LEN (128) /* Number of connections to buffer */
#define SOCK_BUFFER_SIZE  (100)

/**
 * @brief Create socket, bind to given port, and listen.
 *
 * @param port Socket port number
 *
 * @return Server socket on success, ERR on failure.
 */
int tcp_listen(uint16_t port);

/**
 * @brief Wait (block) and accept new connections.
 *
 * @param sock TCP server socket
 * @param csock TCP client socket
 *
 * @return OK on success, ERR on failure.
 */
int tcp_accept(int sock, int * csock);

#endif

