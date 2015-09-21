/*
 * logger.h
 *
 *  Created on: 1 nov. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_KERNEL_LOGGER_H_
#define DARKDNS_KERNEL_LOGGER_H_

#include <time.h>

#include "../buffer/buffer.h"
#include "../conf/conf.h"

typedef struct _Log Log;
typedef struct _LogChildChaine LogChildChaine;
typedef struct _LogChild LogChild;

struct _LogChild{
 Buffer *methode_name;
 Buffer *class_name;
 Buffer *info;
 Log *parent;

 void (*write)(LogChild *myLogChild,char *methode,char *info);
};

struct _LogChildChaine {
 LogChild* me;
 LogChildChaine* next;
};

struct _Log {
 conf_params *params;
 LogChildChaine *childs;
 LogChild* (*child)(Log *myLog,char *className);
};

Log* darkdns_kernel_logger_init(conf_params *params);
void darkdns_kernel_logger_free(Log* myLog);
LogChild* darkdns_kernel_logger_child(Log *myLog,char *className);
void darkdns_kernel_logger_child_free(LogChildChaine* logChildPos);
void darkdns_kernel_logger_write(LogChild *myLogChild,char *methode,char *info);

#endif /* LOGGER_H_ */
