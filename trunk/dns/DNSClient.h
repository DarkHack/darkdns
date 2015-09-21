/*
 * DNSClient.h
 *
 *  Created on: 14 sept. 2013
 *      Author: chapuis
 */

#ifndef DARKDNS_DNS_DNSCLIENT_H_
#define DARKDNS_DNS_DNSCLIENT_H_

#include "dns.h"

#include <stdio.h>
#include <string.h>
#include "../kernel/socket/udp/udp.h"


typedef struct _DNSClient DNSClient;
struct _DNSClient {
 Udp *udp;
 unsigned int server;
 unsigned short int port;

 void (*ask)(DNSClient* DNSClient,char *request);
};

DNSClient* darkdns_dnsclient_init(unsigned int server,unsigned short int port);
void darkdns_dnsclient_ask(DNSClient* DNSClient,char *request);

#endif /* DNSCLIENT_H_ */
