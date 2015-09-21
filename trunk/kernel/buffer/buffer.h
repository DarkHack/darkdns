/*
 * buffer.h
 *
 *  Created on: 20 oct. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_KERNEL_BUFFER_H_
#define DARKDNS_KERNEL_BUFFER_H_

#include <stdlib.h>
#include <stdio.h>
#include "../memory/memory.h"

struct _Bdata;
typedef struct _Bdata Bdata;

struct _Bdata{
 void *data;
 Bdata *pNext;
 Bdata *pPrevious;
};

typedef struct _Buffer Buffer;
struct _Buffer {
 unsigned int elementary;
 unsigned int size;
 Bdata *bData;
 Bdata *bDataFirst;
 Bdata *bDataLast;
 Memory *memory;
 void (*rewind)(Buffer *buffer);
 void (*clear)(Buffer *buffer);
 char (*write)(Buffer *buffer,void *data);
 void (*from_one)(Buffer *buffer,void* data,void* ended,char bAddEnded);
 void* (*read) (Buffer *buffer);
 void* (*in_one)(Buffer *buffer,void* ended);
 void* (*free)(Buffer *buffer);
 void (*copy)(Buffer *buffer,Buffer *from);
};

Buffer* darkdns_kernel_buffer_init(unsigned int size_elementary);
void  darkdns_kernel_buffer_free(Buffer *buffer);
void  darkdns_kernel_buffer_rewind(Buffer *buffer);
void* darkdns_kernel_buffer_read(Buffer *buffer);
void  darkdns_kernel_buffer_clear(Buffer *buffer);
char  darkdns_kernel_buffer_write(Buffer *buffer,void *data);
void darkdns_kernel_buffer_from_one(Buffer *buffer,void* data,void* ended,char bAddEnded);
void* darkdns_kernel_buffer_in_one(Buffer *buffer,void* ended);

#endif /* BUFFER_H_ */
