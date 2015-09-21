/*
 * string.c
 *
 *  Created on: 21 oct. 2012
 *      Author: chapuis
 */

#include "string.h"
/**
 *
 */
char darkdns_kernel_string_compare(Buffer* one,char* two)
{
 char isEquals=0;
 int cpt=0;
 one->rewind(one);
 while(*(two+cpt)!='\0')
 {
  if (*(char*)one->read(one)!=*(two+cpt))
   break;
  cpt++;
 }
 if (cpt==one->size)
  isEquals=1;
 return isEquals;
}
int darkdns_kernel_string_to_int(Buffer* string)
{
 int retour=0,puissance=string->size,base,i;
 char *c;
 string->rewind(string);
 while((c=string->read(string))!=NULL) {
  base=1;
  for(i=1;i<puissance;i++)
  {
   base=base*10;
  }
  if (*c>48 && *c<59)
   retour+=((*c-48)*base);
  puissance--;
 }
 return retour;
}
