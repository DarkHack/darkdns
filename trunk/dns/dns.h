/*
 * dns.h
 *
 *  Created on: 3 nov. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_DNS_DNS_H_
#define DARKDNS_DNS_DNS_H_

#include "../kernel/memory/memory.h"
#include "../kernel/buffer/buffer.h"
#include "../kernel/buffer/androBuffer.h"

char* darkdns_dns_body_read(void *body,unsigned int size);
void darkdns_dns_body_write(androBuffer *theRequest,char* hostname);

/**
 * |-------------------------------------------------|
 * |        Header of an query/answer dns            |
 * |-------------------------------------------------|
 * |                    id 2octets                   |
 * |-------------------------------------------------|
 * |                   flags 2octets                 |
 * |  rd-tc-aa-opcode-qr    |   rcode-cd-ad-z-ra     |
 * |-------------------------------------------------|
 * |                   q_count 2octets               |
 * |-------------------------------------------------|
 * |                 ans_count 2octets               |
 * |-------------------------------------------------|
 * |                 auth_count 2octets              |
 * |-------------------------------------------------|
 * |                 add_count 2octets               |
 * |-------------------------------------------------|
 */
typedef struct {
 unsigned short id; // identification number

 // order of following bits inverted because of little/big endian
 unsigned char recursive :1; // recursion desired
 unsigned char truncated :1; // truncated message
 unsigned char authoritiveAns :1; // authoritive answer
 unsigned char operationCode :4; // purpose of message
 unsigned char isResponse :1; // query/response flag

 // order of following bits inverted because of little/big endian
 unsigned char responseCode :4; // response code
 unsigned char checkingDisabled :1; // checking disabled
 unsigned char authenticatedData :1; // authenticated data
 unsigned char zReserved :1; // its z! reserved
 unsigned char recursionAvailable :1; // recursion available

 unsigned short q_count; // number of question entries
 unsigned short ans_count; // number of answer entries
 unsigned short auth_count; // number of authority entries
 unsigned short add_count; // number of resource entries

}darkdns_dnsHeader;

/**
 * |-------------------------------------------------|
 * |        footer of an query/answer dns            |
 * |-------------------------------------------------|
 * |                 type 2octets                    |
 * |-------------------------------------------------|
 * |                 class 2octets                   |
 * |-------------------------------------------------|
 */
typedef struct {
 unsigned short type; // type
 unsigned short class; // class
}darkdns_dnsFooter;

#define darkdns_dns_opcode_query  0x00
#define darkdns_dns_opcode_rquery 0x01
#define darkdns_dns_opcode_status 0x02

#define darkdns_dns_rcode_noerror   0x00 // no error
#define darkdns_dns_rcode_format    0x01 // querie/response format error
#define darkdns_dns_rcode_server    0x02 // server internal error
#define darkdns_dns_rcode_jhondoe   0x03 // unknow domain
#define darkdns_dns_rcode_dontknow  0x04 // service unknow
#define darkdns_dns_rcode_forbidden 0x05 // forbidden

#define darkdns_dns_class_in 0x0100 // internet
#define darkdns_dns_class_cs 0x0200 // csnet (obselete)
#define darkdns_dns_class_ch 0x0300 // chaos (obselete)
#define darkdns_dns_class_hs 0x0400 // hesiod

#define darkdns_dns_type_a     0x0100  // addresse hote
#define darkdns_dns_type_ns    0x0200 // name server
#define darkdns_dns_type_md    0x0300 // messagerie (obselete)
#define darkdns_dns_type_mf    0x0400 // messagerie (obselete)
#define darkdns_dns_type_cname 0x0500 // other name
#define darkdns_dns_type_soa   0x0600 // info generale de zone
#define darkdns_dns_type_mb    0x0700 // mail of domain zone (experimentale)
#define darkdns_dns_type_mg    0x0800 // member of group mail (experimentale)
#define darkdns_dns_type_mr    0x0900 // alias site (experimentale)
#define darkdns_dns_type_null  0x0A00 // null data
#define darkdns_dns_type_wks   0x0B00 // internet service knowledge
#define darkdns_dns_type_ptr   0x0C00 // resolution inverse
#define darkdns_dns_type_hinfo 0x0D00 // machine describe
#define darkdns_dns_type_minfo 0x0E00 // groupe mail
#define darkdns_dns_type_mx    0x0F00 // mail exchange
#define darkdns_dns_type_txt   0x1000 // string


#endif /* DNS_H_ */
