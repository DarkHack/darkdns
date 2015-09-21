/*
 * kernel.h
 *
 *  Created on: 19 oct. 2012
 *      Author: chapuis
 */

#ifndef DARKKERNEL_KERNEL_H_
#define DARKKERNEL_KERNEL_H_

#include "stdio.h"
#include "memory/memory.h"
#include "buffer/buffer.h"
#include "conf/conf.h"
#include "logger/logger.h"
#include "bdd/bdd.h"


typedef struct {
 void (*params_read)();
 conf* myConf;
 Log* myLog;
 LogChild *myLogChild;
} kernel;
/* constructeur */

kernel* darkdns_kernel_init (void);
void darkdns_kernel_free(kernel* myKernel);
//void darkdns_kernel_init(kernel *myKernel);
void darkdns_kernel_params_read(int argc, char **argv);

#endif /* KERNEL_H_ */
