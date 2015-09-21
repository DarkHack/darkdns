/*
 * memory.h
 *
 *  Created on: 21 oct. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_KERNEL_MEMORY_H_
#define DARKDNS_KERNEL_MEMORY_H_

#include "stdio.h"

#define MALLOC(_p,_t) do{ \
 (_p) = malloc ( (_t) ); \
 if( (_p) == NULL ) \
 { \
  fprintf(stderr,"Allocation impossible dans le fichier :%s ligne : %d",__FILE__,__LINE__);\
  exit(EXIT_FAILURE); \
 } \
} while(0)

#define FREE(_p) do{ \
  free( (_p) ); \
     (_p) = NULL; \
}while(0)

#define darkdns_kernel_memory_size 64456
//32728//16364//8192//4096

typedef struct _BVerif {
 void *bData;
 int size;
} BVerif;

char  darkdns_kernel_memory_space[darkdns_kernel_memory_size];
BVerif darkdns_kernel_memory_map[darkdns_kernel_memory_size];


typedef struct _memory {
 void* (*alloc)(int size);
 char (*free)(void *bData);
 unsigned int (*used)();
} Memory;

Memory* darkdns_kernel_memory_init();
void* darkdns_kernel_memory_alloc(int size);
char  darkdns_kernel_memory_free(void *bData);
char  darkdns_kernel_memory_check_size(void *p,int size);
unsigned int darkdns_kernel_memory_used();

#endif /* MEMORY_H_ */
