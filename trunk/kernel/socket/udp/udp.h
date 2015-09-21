/*
 * udp.h
 *
 *  Created on: 3 nov. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_SOCKET_UDP_UDP_H_
#define DARKDNS_SOCKET_UDP_UDP_H_

#include <unistd.h> // close
#include <arpa/inet.h>

#include "../socket.h"

#include "../../memory/memory.h"
#include "../../buffer/androBuffer.h"

typedef struct _Udp Udp;
struct _Udp{
 SOCKET* sock;
 SOCKADDR_IN* dest;
 int dest_size;

 char (*send)(Udp *udp,androBuffer *theRequest,void* ended);
 char (*receive)(Udp *udp,androBuffer *buffer);
 char (*bind)(Udp *udp);
 void (*free)(Udp *udp);
};

Udp* darkdns_socket_udp_init(unsigned int server,unsigned short int port);
void darkdns_socket_udp_free(Udp *udp);
char darkdns_socket_udp_bind(Udp *udp);
char darkdns_socket_udp_send(Udp *udp,androBuffer *theRequest,void* ended);
char darkdns_socket_udp_receive(Udp *udp,androBuffer *buffer);

#endif /* UDP_H_ */
