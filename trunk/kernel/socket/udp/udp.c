/*
 * udp.c
 *
 *  Created on: 3 nov. 2012
 *      Author: chapuis
 */
#include "udp.h"

Udp* darkdns_socket_udp_init(unsigned int server,unsigned short int port) {
 Udp* self= darkdns_kernel_memory_alloc(sizeof *self);
 char bIsValid=0;

 if (self == NULL)
  fprintf(stderr,"darkdns_socket_udp_init no more memory for self\n");
 else {
  self->sock= darkdns_kernel_memory_alloc(sizeof *self->sock);
  self->dest= darkdns_kernel_memory_alloc(sizeof *self->dest);
  if (self->sock == NULL)
   fprintf(stderr,"darkdns_socket_udp_init no more memory for self->sock\n");
  else if (self->dest == NULL)
   fprintf(stderr,"darkdns_socket_udp_init no more memory for self->dest\n");
  else {
   *self->sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(*self->sock == SOCKET_ERROR)
    fprintf(stderr,"darkdns_socket_udp_init error create socket invalid sock");
   else {
    *self->dest = (SOCKADDR_IN){ 0 };
    self->dest_size = sizeof(*self->dest);

    self->dest->sin_addr.s_addr=server;
    self->dest->sin_port = port;
    self->dest->sin_family = AF_INET;

    self->send=darkdns_socket_udp_send;
    self->receive=darkdns_socket_udp_receive;
    self->bind=darkdns_socket_udp_bind;
    self->free=darkdns_socket_udp_free;
    bIsValid=1;
   }
  }
 }
 if (!bIsValid)
 {
  darkdns_socket_udp_free(self);
  self=NULL;
 }
 return self;
}

void darkdns_socket_udp_free(Udp *udp)
{
 if (udp) {
  if (udp->sock) {
   closesocket(*udp->sock);
   darkdns_kernel_memory_free(udp->sock);
  }
  if (udp->dest) {
   darkdns_kernel_memory_free(udp->dest);
  }
  darkdns_kernel_memory_free(udp);
 }
}
char darkdns_socket_udp_bind(Udp *udp) {
 char bRetour=0;

 if (!udp)
  fprintf(stderr,"darkdns_socket_udp_bind no udp ");
 else {
  udp->dest_size=sizeof(*udp->dest);
  if(bind (*udp->sock, (SOCKADDR *) udp->dest, udp->dest_size) == SOCKET_ERROR)
   fprintf(stderr,"darkdns_socket_udp_bind bind echec");
  else
   bRetour=1;
 }
 return bRetour;
}
char darkdns_socket_udp_send(Udp *udp,androBuffer *buffer,void* ended)
{
 char bValid=0;
 unsigned int
  sended=0
 ,totalSended=0
 ,totalToSend=buffer->size(buffer);

 void *toSend=0x00;

 if (!udp || !buffer)
  fprintf(stderr,"darkdns_socket_udp_send no udp or no buffer");
 else {
  toSend=buffer->in_one(buffer,ended);
  if(!toSend)
   fprintf(stderr,"darkdns_socket_udp_send no more memory for toSend");
  else {
   while(sended>=0 && totalSended<totalToSend) {
    udp->dest_size=sizeof(*udp->dest);
    sended=sendto(*udp->sock, toSend+totalSended, totalToSend-totalSended, 0, (SOCKADDR *)udp->dest, udp->dest_size);
    if( sended < 0)
     fprintf(stderr,"darkdns_socket_udp_send error sendto");
    totalSended+=sended;
   }
   if (totalSended==totalToSend)
    bValid=1;
   darkdns_kernel_memory_free(toSend);
   toSend=0x00;
  }
 }
 return bValid;
}

char darkdns_socket_udp_receive(Udp *udp,androBuffer *buffer)
{
 char octet[1024];
 char bValid=0;
 int n=0;

 if (!udp || !buffer)
  fprintf(stderr,"darkdns_socket_udp_send no udp or no buffer");
 else {
  //do {
   printf("udp wait for reception\n");
   udp->dest_size=sizeof(*udp->dest);
   n = recvfrom(*udp->sock, &octet, sizeof octet, 0, (SOCKADDR *)udp->dest, (socklen_t *)&udp->dest_size);
   if (n>0) {
    buffer->write(buffer,&octet,n);
    printf("udp have receive one octet\n");
   }
  //}while(n>0);
  if(n<0)
   fprintf(stderr,"recvfrom()");
  else
   bValid=1;
 }
 return bValid;
}
