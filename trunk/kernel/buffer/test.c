/*
 * test.c
 *
 *  Created on: 13 sept. 2013
 *      Author: chapuis
 */
#include <stdlib.h>
#include "buffer.h"

int main(int argc, char **argv)
{
 int i,beforeWrite,afterWrite;
 char c;
 void* pointer[100];
 Buffer* bufTest = darkdns_kernel_buffer_init(sizeof(char));
 ///*
 printf("memoire utiliser.\n");
 beforeWrite=bufTest->memory->used();
 printf("ecriture de 10 octect dans le buffer :%d\n",beforeWrite);
 getchar();
 for(i=1;i<10;i++) {
  c=48+i;
  bufTest->write(bufTest,&c);
 }
 c='-';
 bufTest->write(bufTest,&c);
 afterWrite=bufTest->memory->used();
 printf("memoire utiliser :%d\n",afterWrite-beforeWrite);
 getchar();
 //*/
 bufTest->from_one(bufTest,"bonjours tous le monde\n\0","\0","\n");
 printf("sortie du buffer :%s\n",bufTest->in_one(bufTest,"\0"));
 return 1;//sucess
}
