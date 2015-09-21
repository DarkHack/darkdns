/*
 * androBuffer.c
 *
 *  Created on: 2 nov. 2012
 *      Author: chapuis
 */
#include "androBuffer.h"

/**
 * Initialize an buffer with size
 * of an elementary data to stock
 */
androBuffer* darkdns_kernel_androbuffer_init()
{
 static const androBuffer z = {0};

 androBuffer* self = darkdns_kernel_memory_alloc(sizeof *self);

 if (self != 0x00)
 {
  *self = z;
  // attributes
  self->bData=0x00;
  self->bDataFirst=0x00;
  self->bDataLast=0x00;
  // methodes
  self->clear=darkdns_kernel_androbuffer_clear;
  self->read=darkdns_kernel_androbuffer_read;
  self->rewind=darkdns_kernel_androbuffer_rewind;
  self->write=darkdns_kernel_androbuffer_write;
  self->in_one=darkdns_kernel_androbuffer_in_one;
  self->size=darkdns_kernel_androbuffer_size;
  self->free=darkdns_kernel_androbuffer_free;
 }
 return self;
}
/**
 * return an ptr to the next data to read
 * and advance of one the buffer
 */
void* darkdns_kernel_androbuffer_read(androBuffer *buffer)
{
 void *data = 0x00;
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
void darkdns_kernel_androbuffer_rewind(androBuffer *buffer)
{
 buffer->bData=buffer->bDataFirst;
 return ;
}
/**
 * clear all the memory
 */
void darkdns_kernel_androbuffer_clear(androBuffer *buffer)
{
 Bandrodata *previous;
 buffer->bData=buffer->bDataFirst;
 buffer->bDataFirst=0x00;
 buffer->bDataLast=0x00;
 if  (buffer->bData) {
  while (buffer->bData != 0x00)
  {
   darkdns_kernel_memory_free(buffer->bData->data);
   buffer->bData->data=0x00;
   buffer->bData->pPrevious=0x00;

   previous=buffer->bData;
   buffer->bData=buffer->bData->pNext;
   previous->pNext=0x00;
   darkdns_kernel_memory_free(previous);
   previous=0x00;
   //buffer->size--;
  }
  darkdns_kernel_memory_free(buffer->bData);
 }
 return ;
}
void  darkdns_kernel_androbuffer_free(androBuffer *buffer)
{
 if (buffer!=0x00) {
  buffer->clear(buffer);
  darkdns_kernel_memory_free(buffer);
  buffer=0x00;
 }
}
/**
 * write an data to the buffer
 */
char darkdns_kernel_androbuffer_write(androBuffer *buffer,void *data,unsigned int dataSize)
{

 int size=sizeof(Bandrodata),j;
 char bRetour=0;
 // in the end of the buffer
 buffer->bData=buffer->bDataLast;
 if (!buffer->bData) {
  buffer->bData=darkdns_kernel_memory_alloc(size+dataSize);
  buffer->bData->size=dataSize;
  if (buffer->bData==0x00) {
   printf("darkdns_kernel_buffer_write no more memory for write some data will be erase\n");
  }else {
   buffer->bData->data=0x00;
   buffer->bData->pNext=0x00;
   buffer->bData->pPrevious=0x00;
   if (!buffer->bDataFirst)
    buffer->bDataFirst=buffer->bData;
   //buffer->bData->data=buffer->bData+sizeof(Bdata);
  }
 }
 // if I have ever an data
 if (buffer->bData && buffer->bData->data) {
  // i search free memory for newt data
  buffer->bData->pNext=darkdns_kernel_memory_alloc(size+dataSize);
  buffer->bData->pNext->size=dataSize;
  if (buffer->bData->pNext==0x00) {
   printf("darkdns_kernel_buffer_write no more memory for write some data will be erase\n");
  }else {
   buffer->bData->pNext->pPrevious=buffer->bData;
   buffer->bData->pNext->data=0x00;
   buffer->bData->pNext->pNext=0x00;
   buffer->bData=buffer->bData->pNext;
   //buffer->bData->pPrevious=NULL;
   //buffer->bData->data=buffer->bData+sizeof(Bdata);
  }
 }
 if (buffer->bData) {
  buffer->bData->data=buffer->bData;
  buffer->bData->data+=(unsigned int)size;
  // now this is the last
  buffer->bDataLast=buffer->bData;
  //MALLOC(buffer->bData->data,);
 // *(void **)buffer->bData->data=*(void **)data;
  for(j=0;j<buffer->bData->size;j++)
   *(char *)(buffer->bData->data+j)=*(char *)(data+j);
  bRetour=1;
 }
 return bRetour;
}
unsigned int darkdns_kernel_androbuffer_size(androBuffer *buffer) {
 unsigned int size=0;
 Bandrodata *headRead=buffer->bDataFirst;
 while (headRead != 0x00) {
  size+=headRead->size;
  headRead=headRead->pNext;
 }
 return size;
}
/**
 * initialize an big ptr to return in one all the content of the buffer.
 * the end of the buffer can be recognize by the data give in ptr ended.
 */
void* darkdns_kernel_androbuffer_in_one(androBuffer *buffer,void* ended)
{
 void* in_one=0x00;
 unsigned int cpt=0,j;
 Bandrodata *headRead=buffer->bDataFirst;

 in_one=darkdns_kernel_memory_alloc(darkdns_kernel_androbuffer_size(buffer)+1);

 if (in_one!=0x00) {
  buffer->rewind(buffer);
  while (headRead != 0x00) {
   for(j=0;j<headRead->size;j++) {
    *(char *)(in_one+cpt)=*(char *)(headRead->data+j);
    cpt+=0x01;
   }
   headRead=headRead->pNext;
  }
  *(void **)(in_one+cpt)=*(void **)ended;
 }
 return in_one;
}

