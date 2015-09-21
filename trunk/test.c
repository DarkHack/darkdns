/*
 * test.c
 *
 *  Created on: 19 oct. 2012
 *      Author: chapuis
 */

#include "test.h"
#include "dns/DNSServer.h"
#include "dns/DNSClient.h"

int main(int argc, char **argv)
{
 int pid; // PID du processus fils
 kernel *myKernel = darkdns_kernel_init();

 /*
 bdd* bdd = darkkernel_bdd_init(myKernel->myConf->params->psql_info);
 bdd->execute(bdd,"select datname,encoding,datcollate from pg_database");
 bdd->disconnect(bdd);
 */

 // 192.168.1.254:53 => 0xFE01A8C0,0x3500
 // 127.0.0.1:53 => 0x0100007F,0x3500
 // 10.88.16.130 => 0x8210580A
 DNSServer* DNSServer = darkdns_dnsserver_init(0x0500007F,0x3500);
 DNSServer->start(DNSServer);
 /*
 printf("%d :try an requette juste for laught");
 DNSClient* DNSClient = darkdns_dnsclient_init(0x0500007F,0x3500);
 DNSClient->ask(DNSClient,"darknet.fr");
 */
 darkdns_kernel_free(myKernel);
 printf("%d :james brown is dead\n",getpid());
 //*/
 return 1;//sucess
}
