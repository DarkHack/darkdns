/*
 * conf.h
 *
 *  Created on: 19 oct. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_KERNEL_CONF_H_
#define DARKDNS_KERNEL_CONF_H_


#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

#include "../buffer/buffer.h"
#include "../string/string.h"

//char* DARKDNS_KERNEL_CONF_CONFILE="/etc/darkdns/darkdns.conf";

enum {
 psql_info=0
,port_tcp=1
,port_udp=2
,log_file=3
,log_level=4
} DARKDNS_KERNEL_CONF ;

typedef struct {
 char *psql_info;
 int port_udp;
 int port_tcp;
 char *log_file;
 char *conf_file;
 int log_level;
} conf_params;

typedef struct _conf conf;
struct _conf {
 conf_params *params;
 void (*read)(conf *myConf);
 int (*write_exemple)(conf *myConf);
 char* (*absolute_path)(char *file);
} ;

conf* darkdns_kernel_conf_init();
void darkdns_kernel_conf_free(conf* myConf);
char* darkdns_kernel_conf_get_path(char *file);
char* darkdns_kernel_conf_read_line(FILE *fp);
int   darkdns_kernel_conf_analyse_line(conf *myConf,Buffer *bUtilConf);
void darkdns_kernel_conf_read(conf *myConf);
int   darkdns_kernel_conf_write_exemple(conf *myConf);

#endif /* CONF_H_ */
