/*
 * socket.h
 *
 *  Created on: 3 nov. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_SOCKET_SOCKET_H_
#define DARKDNS_SOCKET_SOCKET_H_

#include <sys/socket.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif /* SOCKET_H_ */
