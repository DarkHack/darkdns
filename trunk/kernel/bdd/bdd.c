/*
 * bdd.c
 *
 *  Created on: 13 sept. 2013
 *      Author: chapuis
 */
#include "bdd.h"

/**
 * darkkernel_bdd_init("host = 127.0.0.1 dbname = postgres user = webroot password = *[webroot]*")
 */
bdd* darkkernel_bdd_init(char *conninfo)
{
 static const bdd z = {0};

 bdd* self = darkdns_kernel_memory_alloc(sizeof *self);


 if (self != NULL)
 {
  *self = z;
  // attributes
  self->conninfo = conninfo;
  // methodes
  self->connect=darkkernel_bdd_connect;
  self->disconnect=darkkernel_bdd_disconnect;
  self->execute=darkkernel_bdd_execute;
 }
 return self;
}

/**
 *
 */
char darkkernel_bdd_connect(bdd *self)
{
 char connected=0;

 /* Make a connection to the database */
 self->conn = PQconnectdb(self->conninfo);
 if (PQstatus(self->conn) != CONNECTION_OK) {
  fprintf(stderr, "Connection to database failed: %s",PQerrorMessage(self->conn));
  self->disconnect(self);
 } else {
  connected=1;
 }
 return connected;
}

/**
 *
 */
void darkkernel_bdd_disconnect(bdd *self)
{
 PQfinish(self->conn);
 self->conn=0x00;
}
/**
 * query = select * from pg_database
 */
char darkkernel_bdd_execute(bdd *self,char *query)
{
 PGresult   *res;
 char portal[1000],result=1;

 int         nFields;
 int         i,
             j;

 if (!self->conn)
  self->connect(self);

 if (!self->conn)
  return 0;

 /* Start a transaction block */
 res = PQexec(self->conn, "BEGIN");
 if (PQresultStatus(res) != PGRES_COMMAND_OK)
 {
     fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(self->conn));
     PQclear(res);
     self->disconnect(self);
     result=0;
 }
 PQclear(res);

 /*
  * Fetch rows from pg_database, the system catalog of databases
  */
 sprintf(portal, "DECLARE myportal CURSOR FOR %s\0", query);
 res = PQexec(self->conn, portal);
 if (PQresultStatus(res) != PGRES_COMMAND_OK)
 {
     fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(self->conn));
     PQclear(res);
     PQfinish(self->conn);
 }
 PQclear(res);

 res = PQexec(self->conn, "FETCH ALL in myportal");
 if (PQresultStatus(res) != PGRES_TUPLES_OK)
 {
     fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(self->conn));
     PQclear(res);
     PQfinish(self->conn);
 }

 /* first, print out the attribute names */
 nFields = PQnfields(res);
 for (i = 0; i < nFields; i++)
     printf("%-15s", PQfname(res, i));
 printf("\n\n");

 /* next, print out the rows */
 for (i = 0; i < PQntuples(res); i++)
 {
     for (j = 0; j < nFields; j++)
         printf("%-15s", PQgetvalue(res, i, j));
     printf("\n");
 }

 PQclear(res);

 /* close the portal ... we don't bother to check for errors ... */
 res = PQexec(self->conn, "CLOSE myportal");
 PQclear(res);

 /* end the transaction */
 res = PQexec(self->conn, "END");
 PQclear(res);

 self->disconnect(self);

 return result;
}
