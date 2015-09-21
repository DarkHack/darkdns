/*
 * androBuffer.h
 *
 *  Created on: 2 nov. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_KERNEL_ANDROBUFFER_H_
#define DARKDNS_KERNEL_ANDROBUFFER_H_

#include "stdlib.h"
#include "stdio.h"
#include "../memory/memory.h"

//struct _Bandrodata;
typedef struct _Bandrodata Bandrodata;

struct _Bandrodata{
 void *data;
 unsigned int size;
 Bandrodata *pNext;
 Bandrodata *pPrevious;
};

typedef struct _androBuffer androBuffer;
struct _androBuffer {
 Bandrodata *bData;
 Bandrodata *bDataFirst;
 Bandrodata *bDataLast;
 void (*rewind)(androBuffer *buffer);
 void (*clear)(androBuffer *buffer);
 char (*write)(androBuffer *buffer,void *data,unsigned int dataSize);
 void* (*read) (androBuffer *buffer);
 void (*free) (androBuffer *buffer);
 unsigned int (*size)(androBuffer *buffer);
 void* (*in_one)(androBuffer *buffer,void* ended);
};

androBuffer* darkdns_kernel_androbuffer_init();
void  darkdns_kernel_androbuffer_free(androBuffer *buffer);
void  darkdns_kernel_androbuffer_rewind(androBuffer *buffer);
void* darkdns_kernel_androbuffer_read(androBuffer *buffer);
void  darkdns_kernel_androbuffer_clear(androBuffer *buffer);
unsigned int darkdns_kernel_androbuffer_size(androBuffer *buffer);
char darkdns_kernel_androbuffer_write(androBuffer *buffer,void *data,unsigned int dataSize);
void* darkdns_kernel_androbuffer_in_one(androBuffer *buffer,void* ended);

#endif /* ANDROBUFFER_H_ */
