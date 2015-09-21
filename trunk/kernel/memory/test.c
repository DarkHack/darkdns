/*
 * test.c
 *
 *  Created on: 13 sept. 2013
 *      Author: chapuis
 */
#include <stdlib.h>
#include "memory.h"

int main(int argc, char **argv)
{
 int i;
 void* pointer[100];
 Memory* memTest = darkdns_kernel_memory_init();
 memTest->used();
 getchar();
 for(i=0;i<100;i++) {
  printf("%d\n",i);
  pointer[i]=memTest->alloc(100);
  memTest->used();
 }
 memTest->used();
 printf("ecriture et remplissage memoire fini.");
 getchar();
 for(i=0;i<100;i++) {
  printf("%d\n",i);
  memTest->free(pointer[i]);
  memTest->used();
 }
 return 1;//sucess
}
