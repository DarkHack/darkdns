/*
 * test.c
 *
 *  Created on: 13 sept. 2013
 *      Author: chapuis
 */
#include <stdlib.h>
#include "bdd.h"

int main(int argc, char **argv)
{
 bdd* bdd = darkkernel_bdd_init("host = 127.0.0.1 dbname = postgres user = webroot password = *[webroot]*");

 bdd->connect(bdd);
 bdd->execute(bdd,"select datname,encoding,datcollate from pg_database");
 bdd->disconnect(bdd);

 return 1;//sucess
}
