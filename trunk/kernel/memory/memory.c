/*
 * memory.c
 *
 *  Created on: 21 oct. 2012
 *      Author: chapuis
 */
#include "memory.h"

Memory* darkdns_kernel_memory_init()
{
 static char hasBeenInit=0;
 int i,progress=0;
 BVerif *myVerif;
 if (!hasBeenInit) {
  for(i=0;i<darkdns_kernel_memory_size;i++) {
   if (((int)((float)((float)i/(float)darkdns_kernel_memory_size)*100))>=(progress+30)) {
    progress=((int)((float)((float)i/(float)darkdns_kernel_memory_size)*100));
    printf("memory init :%d /100 \n",progress);
   }
   myVerif=&darkdns_kernel_memory_map[i];
   myVerif->size=0;
   myVerif->bData=0x00;
  }
  hasBeenInit=1;
  printf("memory init :100 / 100 \n");
  printf("memory total :%d octets\n",darkdns_kernel_memory_size);
 }
 Memory *self=darkdns_kernel_memory_alloc(sizeof *self);
 if (self)
 {
  self->alloc=darkdns_kernel_memory_alloc;
  self->free=darkdns_kernel_memory_free;
  self->used=darkdns_kernel_memory_used;
 }
 return self;
}
unsigned int darkdns_kernel_memory_used()
{
 unsigned int nRetour=0;//,maxSpace=0;
 int i;
 BVerif *myVerif;

 for(i=0;i<darkdns_kernel_memory_size;i++) {
  myVerif=&darkdns_kernel_memory_map[i];
  nRetour+=myVerif->size;
 }
 printf("------------------------\n");
 printf("memory used\t:%d octets\n",nRetour);
 printf("memory free\t:%d octets\n",darkdns_kernel_memory_size-nRetour);
 printf("memory state\t: %d / 100\n",((int)((float)((float)nRetour/(float)darkdns_kernel_memory_size)*100)));
 printf("------------------------\n");
 return nRetour;
}

void* darkdns_kernel_memory_alloc(int size)
{
 char bRetour=0;
 int i,j;
 BVerif *myVerif;
 void *bData=0x00;

 bData=(void*)(&darkdns_kernel_memory_space[0]);
 for(i=0;i<darkdns_kernel_memory_size;i++) {
  myVerif=&darkdns_kernel_memory_map[i];
  if (myVerif->bData!=0x00)
   bData=myVerif->bData;
  for(j=0;j<=myVerif->size && myVerif->size>0;j++)
   bData+=0x01;
  if (myVerif->size==0 && darkdns_kernel_memory_check_size(bData,size) && bData<=((void*)(&darkdns_kernel_memory_space[0])+darkdns_kernel_memory_size)) {
   myVerif->size=size;
   myVerif->bData=bData;
   bRetour=1;
   break;
  }
 }
 if (!bRetour || myVerif->bData==0x00) {
  bData=0x00;
  darkdns_kernel_memory_used();
  printf("darkdns_kernel_memory_alloc no more free ask :%d\n",size);
 }
 return bData;
}
char darkdns_kernel_memory_check_size(void *p,int size)
{
 char bRetour=0;
 int i,totalFreeSize=darkdns_kernel_memory_size,freeSize=darkdns_kernel_memory_size;
 BVerif *myVerif;
 for(i=0;i<darkdns_kernel_memory_size;i++) {
  myVerif=&darkdns_kernel_memory_map[i];
  totalFreeSize-=myVerif->size;
  if (myVerif->bData>p && (myVerif->bData-p)<freeSize)
   freeSize=myVerif->bData-p;
 }
 if (freeSize>=size && totalFreeSize>=size)
  bRetour=1;
 return bRetour;
}
char darkdns_kernel_memory_free(void *bData)
{
 char bRetour=0;
 int i;
 BVerif *myVerif;

 for(i=0;i<darkdns_kernel_memory_size;i++) {
  myVerif=&darkdns_kernel_memory_map[i];
  if (myVerif->bData==bData)
  {
   myVerif->bData=0x00;
   myVerif->size=0;
  }
 }
 return bRetour;
}


