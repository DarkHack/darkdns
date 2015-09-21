/*
 * dns.c
 *
 *  Created on: 14 sept. 2013
 *      Author: chapuis
 */
#include "dns.h"
/**
 * @param : receive, the body of request
 * @param : hostname to construct
 *
 * this function, make the body for dns query/answer
 * to string
 */
char* darkdns_dns_body_read(void *body,unsigned int size) {
 void *bodyReader;
 Buffer *bufHostname = darkdns_kernel_buffer_init(sizeof(char));
 char isData=0,myLength=0,*hostname;

 bodyReader=body+(unsigned int)(sizeof(darkdns_dnsHeader));
 for(
  bodyReader=body+(unsigned int)(sizeof(darkdns_dnsHeader));
  (bodyReader-(body+(unsigned int)(sizeof(darkdns_dnsHeader))))<size;
  bodyReader+=0x01
 ) {
  //printf("%c/%d|",(*(char*)bodyReader),(*(char*)bodyReader));
  if (myLength>0 && isData) {
   myLength--;
   bufHostname->write(bufHostname,bodyReader);
  } else if (!isData || myLength==0) {
   myLength=(*(char*)bodyReader);
   isData=1;
   if (bufHostname->size>0 && myLength>0 && (bodyReader-(body+(unsigned int)(sizeof(darkdns_dnsHeader))))<(size-1))
    bufHostname->write(bufHostname,".");
  }
 }
 hostname=bufHostname->in_one(bufHostname,"\0");
 //bufHostname->free(bufHostname);
 return hostname;
}
/**
 * @param : androBuffer *theRequest, buffer de sortie
 * @param : hostname to construct
 *
 * this function, make the formation for dns query/answer :
 * eg:
 *  google.fr => 6google2fr
 *  darkweb.fr => 7darkweb2fr
 *  google.com => 6google3com
 */
void darkdns_dns_body_write(androBuffer *theRequest,char* hostname) {
 char *testReadAsking=0x00;
 char *testWriteAsking=0x00;
 char *testWriteAskingSize=0x00;
 int totalLength=0;

 // we count how many character the hostname have
 totalLength=0;
 testReadAsking=hostname;
 while(*testReadAsking!='\0') {
  totalLength+=1;
  testReadAsking+=0x01;
 }

 // the size of the query, is the length of hostname more 1
 // more 1 for ended character
 int queriesize=totalLength+1;
 // alloc the memory for total queries
 void *request=(void*)darkdns_kernel_memory_alloc(queriesize);
 testReadAsking=hostname;
 testWriteAsking=(char *)request;
 testWriteAskingSize=testWriteAsking;
 testWriteAsking+=0x01;
 testReadAsking=hostname;
 totalLength=0;
 while(*testReadAsking!='\0') {
  if (*testReadAsking!='.') {
   *testWriteAsking=*testReadAsking;
   totalLength+=1;
  } else {
   *testWriteAskingSize=(char)totalLength;
   testWriteAskingSize=testWriteAsking;
   totalLength=0;
  }
  testWriteAsking+=0x01;
  testReadAsking+=0x01;
 }
 *testWriteAsking=0x00;
 *testWriteAskingSize=(char)totalLength;

 theRequest->write(theRequest,request,queriesize);
 darkdns_kernel_memory_free(request);
 //theRequest->write(theRequest,"t",1);
 theRequest->write(theRequest,"\x00",1);
}
