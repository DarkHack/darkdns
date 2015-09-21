/*
 * buffer.c
 *
 *  Created on: 20 oct. 2012
 *      Author: chapuis
 */
#include "buffer.h"
/**
 * Initialize an buffer with size
 * of an elementary data to stock
 */
Buffer* darkdns_kernel_buffer_init(unsigned int size_elementary)
{
 static const Buffer z = {0};
 Memory* memory=darkdns_kernel_memory_init();

 Buffer* self = memory->alloc(sizeof *self);

 if (self != NULL)
 {
  *self = z;
  // attributes
  self->elementary=size_elementary;
  self->memory=memory;
  self->size=0;
  self->bData=NULL;
  self->bDataFirst=NULL;
  self->bDataLast=NULL;
  // methodes
  self->clear=darkdns_kernel_buffer_clear;
  self->read=darkdns_kernel_buffer_read;
  self->rewind=darkdns_kernel_buffer_rewind;
  self->write=darkdns_kernel_buffer_write;
  self->in_one=darkdns_kernel_buffer_in_one;
  self->from_one=darkdns_kernel_buffer_from_one;
 }
 return self;
}
/**
 * return an ptr to the next data to read
 * and advance of one the buffer
 */
void* darkdns_kernel_buffer_read(Buffer *buffer)
{
 void *data = NULL;
 if (buffer->bData)
 {
  data=buffer->bData->data;
  buffer->bData=buffer->bData->pNext;
 }
 return data;
}
/**
 * rewind in begin the buffer
 */
void darkdns_kernel_buffer_rewind(Buffer *buffer)
{
 buffer->bData=buffer->bDataFirst;
 return ;
}
/**
 * clear all the memory
 */
void darkdns_kernel_buffer_clear(Buffer *buffer)
{
 Bdata *previous;
 buffer->bData=buffer->bDataFirst;
 buffer->bDataFirst=NULL;
 buffer->bDataLast=NULL;
 if  (buffer->bData) {
  while (buffer->bData != NULL)
  {
   buffer->memory->free(buffer->bData->data);
   buffer->bData->data=NULL;
   buffer->bData->pPrevious=NULL;

   previous=buffer->bData;
   buffer->bData=buffer->bData->pNext;
   previous->pNext=NULL;
   buffer->memory->free(previous);
   previous=NULL;
   //buffer->size--;
  }
  buffer->memory->free(buffer->bData);
 }
 buffer->size=0;
 return ;
}
void  darkdns_kernel_buffer_free(Buffer *buffer)
{
 if (buffer!=NULL) {
  buffer->clear(buffer);
  buffer->memory->free(buffer);
  buffer=0x00;
 }
}
/**
 * write an data to the buffer
 */
char darkdns_kernel_buffer_write(Buffer *buffer,void *data)
{

 int size=sizeof(Bdata),i;
 char bRetour=0;
 // in the end of the buffer
 buffer->bData=buffer->bDataLast;
 if (!buffer->bData) {
  buffer->bData=buffer->memory->alloc(size+buffer->size);
  if (buffer->bData==NULL) {
   printf("darkdns_kernel_buffer_write no more memory for write some data will be erase\n");
  }else {
   buffer->bData->data=NULL;
   buffer->bData->pNext=NULL;
   buffer->bData->pPrevious=NULL;
   if (!buffer->bDataFirst)
    buffer->bDataFirst=buffer->bData;
   //buffer->bData->data=buffer->bData+sizeof(Bdata);
   buffer->size++;
  }
 }
 // if I have ever an data
 if (buffer->bData && buffer->bData->data) {
  // i search free memory for newt data
  buffer->bData->pNext=buffer->memory->alloc(size+buffer->size);
  if (buffer->bData->pNext==NULL) {
   printf("darkdns_kernel_buffer_write no more memory for write some data will be erase\n");
  }else {
   buffer->bData->pNext->pPrevious=buffer->bData;
   buffer->bData->pNext->data=NULL;
   buffer->bData->pNext->pNext=NULL;
   buffer->bData=buffer->bData->pNext;
   //buffer->bData->pPrevious=NULL;
   //buffer->bData->data=buffer->bData+sizeof(Bdata);
   buffer->size++;
  }
 }
 if (buffer->bData) {
  buffer->bData->data=buffer->bData;
  for(i=0;i<size;i++)
   buffer->bData->data+=0x01;
  // now this is the last
  buffer->bDataLast=buffer->bData;
  //MALLOC(buffer->bData->data,);
  *(void **)buffer->bData->data=*(void **)data;
  bRetour=1;
 }
 return bRetour;
}
/**
 * initialize an big ptr to return in one all the content of the buffer.
 * the end of the buffer can be recognize by the data give in ptr ended.
 */
void* darkdns_kernel_buffer_in_one(Buffer *buffer,void* ended)
{
 void* in_one=0x00;
 void* p=0x00;
 unsigned int cpt=0;
 //if (buffer->elementary*(buffer->size+1)<5)
 // printf("attention\n");
 in_one=buffer->memory->alloc((buffer->size+1)*buffer->elementary);
 if (in_one!=NULL) {
  buffer->rewind(buffer);
  while((p=buffer->read(buffer))) {
   *(void **)(in_one+cpt)=*(void **)p;
   cpt+=buffer->elementary;
  }
  *(void **)(in_one+cpt)=*(void **)ended;
 }
 return in_one;
}
/**
 * initialize an buffer with all data in ptr.
 * recognize the end by ended and if bAddEnded is true
 * and the data ended to end of the buffer.
 */
void darkdns_kernel_buffer_from_one(Buffer *buffer,void* data,void* ended,char bAddEnded)
{
 unsigned int cpt=0,i;
 char isIdent=0;
 // *(void **)(data+cpt)!=*(void **)ended
 do {
  isIdent=1;
  for(i=0;i<buffer->elementary && isIdent;i++)
   isIdent=(*(char*)(void **)(data+cpt)==*(char*)(void **)ended);
  if (!isIdent) {
   if (!buffer->write(buffer,data+cpt))
    fprintf(stderr,"darkdns_kernel_buffer_from_one ecriture buffer echouer\n");
   cpt+=buffer->elementary;
  }
 }while(!isIdent);
 if (bAddEnded)
  if (!buffer->write(buffer,ended))
  fprintf(stderr,"darkdns_kernel_buffer_from_one ecriture buffer echouer\n");
}
