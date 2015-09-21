/*
 * kernel.c
 *
 *  Created on: 19 oct. 2012
 *      Author: chapuis
 */
#include "kernel.h"

/**
 init the kernel
 */
kernel* darkdns_kernel_init (void)
{
 static const kernel z = {0};

 darkdns_kernel_memory_init();
 kernel* self = darkdns_kernel_memory_alloc(sizeof *self);

 if (self == NULL)
  printf("No more memory for kernel object\n");
 else {
  *self = z;
  //method of params read
  self->params_read=darkdns_kernel_params_read;
  // read of configuration for kernel conf attribute
  self->myConf=darkdns_kernel_conf_init();
  if (self->myConf==NULL) {
   printf("No more memory in kernel for conf object\n");
   darkdns_kernel_free(self);
   self=0x00;
  } else {
   self->myConf->read(self->myConf);
   ///*
   self->myLog=darkdns_kernel_logger_init(self->myConf->params);
   if (self->myLog==NULL) {
    printf("No more memory in kernel for log object\n");
    darkdns_kernel_free(self);
    self=0x00;
    darkdns_kernel_memory_used();
   }else {
    darkdns_kernel_memory_used();
    ///*
    self->myLogChild=self->myLog->child(self->myLog,"kernel");
    darkdns_kernel_memory_used();
    if (self->myLogChild==NULL) {
     printf("No more memory in kernel for logChild object\n");
     darkdns_kernel_free(self);
     self=0x00;
    }else {
     self->myLogChild->write(self->myLogChild,"darkdns_kernel_init","coucou");
    }
    //*/
   }
  }
  //*/
 }
 return self;
}
void darkdns_kernel_free(kernel* myKernel)
{
 if (myKernel!=NULL) {
  if (myKernel->myLog!=NULL) {
   darkdns_kernel_logger_free(myKernel->myLog);
   myKernel->myLog=0x00;
  }
  if (myKernel->myConf!=NULL) {
   darkdns_kernel_conf_free(myKernel->myConf);
   myKernel->myConf=0x00;
  }
  darkdns_kernel_memory_free(myKernel);
  myKernel=0x00;
 }
 darkdns_kernel_memory_used();
}

void darkdns_kernel_params_read(int argc, char **argv)
{
 int params_cpt;
 for(params_cpt=0;params_cpt<argc;params_cpt++)
 {
  printf("params :%d %s\n",params_cpt,*(argv+params_cpt));
 }
 return ;
}
