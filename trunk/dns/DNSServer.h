/*
 * server.h
 *
 *  Created on: 14 sept. 2013
 *      Author: chapuis
 */

#ifndef DARKDNS_DNS_DNSSERVER_H_
#define DARKDNS_DNS_DNSSERVER_H_

#include "../kernel/memory/memory.h"
#include "../kernel/buffer/androBuffer.h"
#include "../kernel/buffer/buffer.h"

#include "dns.h"

#include <stdio.h>
#include <string.h>
#include "../kernel/socket/udp/udp.h"


typedef struct _DNSServer DNSServer;
struct _DNSServer {
 Udp *udp;
 __pid_t pid;
 unsigned int server;
 unsigned short int port;

 void (*start)(DNSServer* DNSServer);
 void (*run)(DNSServer* DNSServer);
 void (*constructBody)(androBuffer *theRequest,char* requette);
 void (*execute)(DNSServer* DNSServer,androBuffer *theRequest);
};

DNSServer* darkdns_dnsserver_init(unsigned int server,unsigned short int port);
void darkdns_dnsserver_start(DNSServer* DNSServer);
void darkdns_dnsserver_run(DNSServer* DNSServer);
void darkdns_dnsserver_execute(DNSServer* DNSServer,androBuffer *theRequest);
void darkdns_dnsserver_analyseBody(void *receive,unsigned int sizeBody);

#endif /* DNSSERVER_H_ */
