/*
 * DNSClient.c
 *
 *  Created on: 14 sept. 2013
 *      Author: chapuis
 */
#include "DNSClient.h"

DNSClient* darkdns_dnsclient_init(unsigned int server,unsigned short int port) {
 static const DNSClient z = {0};

 DNSClient* self = darkdns_kernel_memory_alloc(sizeof *self);

 if (self == 0x00)
  printf("No more memory for DNSClient object\n");
 else {
  *self = z;
  self->udp = darkdns_socket_udp_init(server,port);

  if (self->udp==0x00) {
   printf("darkdns_dnsclient_init, echec in create udp object\n");
   //darkdns_kernel_free(self);
   self=0x00;
  } else {
   self->server=server;
   self->port=port;
   self->ask=darkdns_dnsclient_ask;
  }
 }
 return self;
}

void darkdns_dnsclient_ask(DNSClient* DNSClient,char *request) {

 androBuffer *theRequest = darkdns_kernel_androbuffer_init();
 void *received;
 char *octet;

 darkdns_dnsHeader *dnsHeader=darkdns_kernel_memory_alloc(sizeof(*dnsHeader));
 darkdns_dnsFooter *dnsFooter=darkdns_kernel_memory_alloc(sizeof(*dnsFooter));
 dnsHeader->id=0x01bc;

 dnsHeader->recursive=1;
 dnsHeader->truncated=0;
 dnsHeader->authoritiveAns=0;
 dnsHeader->operationCode=darkdns_dns_opcode_query;
 dnsHeader->isResponse=0;

 dnsHeader->responseCode=darkdns_dns_rcode_noerror;
 dnsHeader->checkingDisabled=0;
 dnsHeader->authenticatedData=0;
 dnsHeader->zReserved=0;
 dnsHeader->recursionAvailable=0;

 dnsHeader->q_count=1 << 8;
 dnsHeader->ans_count=0x0000;
 dnsHeader->auth_count=0x0000;
 dnsHeader->add_count=0x0000;

 dnsFooter->type=darkdns_dns_type_a;
 dnsFooter->class=darkdns_dns_class_in;

 theRequest->write(theRequest,dnsHeader,sizeof(*dnsHeader));
 darkdns_dns_body_write(theRequest,request);
 theRequest->write(theRequest,dnsFooter,sizeof(*dnsFooter));

 DNSClient->udp->send(DNSClient->udp,theRequest,"\x00");

 darkdns_kernel_memory_free(dnsHeader);
 darkdns_kernel_memory_free(dnsFooter);
 theRequest->clear(theRequest);

 DNSClient->udp->receive(DNSClient->udp,theRequest);
 received=theRequest->read(theRequest);

 //int n=theRequest->size(theRequest);
 //unsigned int sizeBody =(n-sizeof(*dnsFooter)-sizeof(*dnsHeader));

 dnsHeader=(darkdns_dnsHeader *)received;
 /*
 theRequest->rewind(theRequest);
 printf("donc conclusion nous avons :\n");
 do {
  octet=theRequest->read(theRequest);
  if (octet!=0x00)
  printf("%c",*octet);

 }while(octet!=0x00);
 printf("\n",*octet);
 /*
 theRequest->write(theRequest,buffer,sizeof(*dnsHeader));

 theRequest->clear(theRequest);

 theRequest->write(theRequest,buffer+sizeof(*dnsHeader),sizeBody);
 bufRequest=theRequest->read(theRequest);
 theRequest->clear(theRequest);

 theRequest->write(theRequest,buffer+(n-sizeof(*dnsFooter)),sizeof(*dnsFooter));
 dnsFooter=theRequest->read(theRequest);
 theRequest->clear(theRequest);
 */
 darkdns_kernel_androbuffer_free(theRequest);
 DNSClient->udp->free(DNSClient->udp);
}

