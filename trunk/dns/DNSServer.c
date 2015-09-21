/*
 * DNSServer.c
 *
 *  Created on: 14 sept. 2013
 *      Author: chapuis
 */
#include "DNSServer.h"

DNSServer* darkdns_dnsserver_init(unsigned int server,unsigned short int port) {
 static const DNSServer z = {0};

 DNSServer* self = darkdns_kernel_memory_alloc(sizeof *self);

 if (self == 0x00)
  printf("No more memory for DNSServer object\n");
 else {
  *self = z;
  self->udp = darkdns_socket_udp_init(server,port);

  if (self->udp==0x00) {
   printf("darkdns_dnsserver_init, echec in create udp object\n");
   //darkdns_kernel_free(self);
   self=0x00;
  } else {
   self->pid=0;
   self->server=server;
   self->port=port;
   self->start=darkdns_dnsserver_start;
   self->run=darkdns_dnsserver_run;
   self->execute=darkdns_dnsserver_execute;
  }
 }
 return self;
}

void darkdns_dnsserver_start(DNSServer* DNSServer) {
 if (DNSServer->pid!=0)
  printf("%d-darkdns_dnsserver_start : server ever started", getpid());
 else {
  DNSServer->pid = fork();
  switch (DNSServer->pid) {
  case -1:
      printf("%d-darkdns_dnsserver_start : Erreur: echec du fork()\n", getpid());
      break;
  case 0:
      // processus fils
      printf("%d-darkdns_dnsserver_start : the server has been launch, i'm gone\n", getpid() );
      break;
  default:
      // processus pere
      printf("%d-darkdns_dnsserver_start :server started by pid=%d\n", getpid(), DNSServer->pid );

      DNSServer->udp->dest->sin_addr.s_addr=DNSServer->server;//htonl(INADDR_ANY);
      DNSServer->udp->dest->sin_family = AF_INET;
      DNSServer->udp->dest->sin_port = DNSServer->port;//htons(53);
      DNSServer->run(DNSServer);

  }
 }
}

void darkdns_dnsserver_run(DNSServer* DNSServer) {
 androBuffer *theRequest = darkdns_kernel_androbuffer_init();
 int pid;

 DNSServer->udp->bind(DNSServer->udp);
 while(DNSServer->udp->receive(DNSServer->udp,theRequest)) {
  pid = fork();
  switch (pid) {
  case -1:
      printf("%d-darkdns_dnsserver_run:Erreur: echec du fork()\n",getpid());
      break;
  case 0:
      // processus fils
      printf("%d-darkdns_dnsserver_run:children, has go to die for let's father controler\n", getpid() );
      break;
  default:
      // processus pere
      printf("%d-darkdns_dnsserver_run:now my children is here, I go to execute the request\n", getpid() );
      DNSServer->execute(DNSServer,theRequest);
      printf("%d-darkdns_dnsserver_run:all is right\n",getpid());
  }
  if(pid==0)
   break;
  printf("%d-darkdns_dnsserver_run:I wait my wife for make my children pid:%d\n", getpid(),pid );
 };
 if(pid!=0) {
  printf("%d-darkdns_dnsserver_run:ended,pid:%d\n",getpid(),pid);
  darkdns_kernel_androbuffer_free(theRequest);
  DNSServer->udp->free(DNSServer->udp);
 }
}

void darkdns_dnsserver_execute(DNSServer* DNSServer,androBuffer *theRequest) {

 androBuffer *theResponse = darkdns_kernel_androbuffer_init();
 Buffer *theBody = darkdns_kernel_buffer_init(sizeof(char));

 void *receive;
 int n;
 unsigned int sizeBody;
 receive=theRequest->read(theRequest);
 darkdns_dnsHeader *dnsHeader;
 darkdns_dnsFooter *dnsFooter;
 n=theRequest->size(theRequest);
 sizeBody =(n-sizeof(*dnsFooter)-sizeof(*dnsHeader));

 dnsHeader=receive;
 printf("body of the request :%s\n",darkdns_dns_body_read(receive,sizeBody));
 //darkdns_dns_body_read(receive,sizeBody);
 dnsFooter=receive+(unsigned int)(n-sizeof(*dnsFooter));

 dnsHeader->isResponse=1;
 dnsHeader->ans_count=2 << 8;

 theResponse->write(theResponse,dnsHeader,sizeof(*dnsHeader));
 theResponse->write(theResponse,receive+(unsigned int)(sizeof(*dnsHeader)),sizeBody);
 theResponse->write(theResponse,dnsFooter,sizeof(*dnsFooter));

 // reponse A
 darkdns_dns_body_write(theResponse,"google.darkweb.fr");
 theResponse->write(theResponse,dnsFooter,sizeof(*dnsFooter));
 // Time to live
 theResponse->write(theResponse,"\x00\x00\x00\xf6",4);
 // length = 4octet
 theResponse->write(theResponse,"\x00\x04",2);
 // ip 4octet 173.194.78.94
 theResponse->write(theResponse,"\xad\xc2\x4e\x5e",4);

 // reponse B
 darkdns_dns_body_write(theResponse,"google.darknet.fr");
 theResponse->write(theResponse,dnsFooter,sizeof(*dnsFooter));
 // Time to live
 theResponse->write(theResponse,"\x00\x00\x00\xf6",4);
 // length = 4octet
 theResponse->write(theResponse,"\x00\x04",2);
 // ip 4octet 173.194.78.94
 theResponse->write(theResponse,"\xad\xc2\x4b\x5b",4);

 theRequest->clear(theRequest);
 DNSServer->udp->send(DNSServer->udp,theResponse,"\0");

 theResponse->clear(theResponse);
 darkdns_kernel_memory_free(receive);
 darkdns_kernel_androbuffer_free(theResponse);
}


