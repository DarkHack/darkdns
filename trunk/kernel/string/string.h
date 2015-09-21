/*
 * string.h
 *
 *  Created on: 21 oct. 2012
 *      Author: chapuis
 */

#ifndef DARKDNS_KERNEL_STRING_H_
#define DARKDNS_KERNEL_STRING_H_

#include "../buffer/buffer.h"

char darkdns_kernel_string_compare(Buffer* one,char* two);
int darkdns_kernel_string_to_int(Buffer* string);

#endif /* STRING_H_ */
