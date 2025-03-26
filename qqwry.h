/*
 * Copyright 2008 Surf Chen <http://www.surfchen.org>
 *
 *
 * This source code is under the terms of the
 * GNU Lesser General Public License. 
 * see <http://www.gnu.org/licenses/lgpl.txt>
 */

#include <inttypes.h>
#include <cstdio>
extern "C"
{
	int qqwry_get_location(char* addr1, char* addr2, const char* ip, FILE* qqwry_file);
	int qqwry_get_location_by_long(char* addr1, char* addr2, const uint32_t ip, FILE* qqwry_file);
}

