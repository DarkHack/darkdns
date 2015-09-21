/*
 * kernel.h
 *
 *  Created on: 13 sept. 2013
 *      Author: chapuis
 */

#ifndef DARKKERNEL_BDD_H_
#define DARKKERNEL_BDD_H_

#include <stdlib.h>
#include <stdio.h>
#include <libpq-fe.h>

#include "../memory/memory.h"
#include "../buffer/buffer.h"
//#include "conf/conf.h"
//#include "logger/logger.h"


typedef struct _bdd bdd;
struct _bdd {
 PGconn *conn;
 char *conninfo;
 char (*connect)(bdd *self);
 void (*disconnect)(bdd *self);
 char (*execute)(bdd *self,char *query);
};
/* constructeur */

bdd* darkkernel_bdd_init(char *conninfo);
char darkkernel_bdd_connect(bdd *self);
void darkkernel_bdd_disconnect(bdd *self);
char darkkernel_bdd_execute(bdd *self,char *query);
#endif /* DARKKERNEL_BDD_H_ */
