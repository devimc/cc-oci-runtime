// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "utils.h"

/*!
 * Set file descriptor as non-blocking
 *
 * \param fd File descriptor to set as non-blocking
 */
bool
set_fd_nonblocking(int fd)
{
	int flags;

	if (fd < 0) {
		return false;
	}

	flags = fcntl(fd, F_GETFL);
	if (flags == -1) {
		shim_error("Error getting status flags for fd: %s\n", strerror(errno));
		return false;
	}
	flags |= O_NONBLOCK;

	if (fcntl(fd, F_SETFL, flags) == -1) {
		shim_error("Error setting fd as nonblocking: %s\n", strerror(errno));
		return false;
	}
	return true;
}

/*!
 * Store integer as big endian in buffer
 *
 * \param buf Buffer to store the value in
 * \param val Integer to be converted to big endian
 */
void
set_big_endian_32(uint8_t *buf, uint32_t val)
{
        buf[0] = (uint8_t)(val >> 24);
        buf[1] = (uint8_t)(val >> 16);
        buf[2] = (uint8_t)(val >> 8);
        buf[3] = (uint8_t)val;
}

/*!
 * Convert the value stored in buffer to little endian
 *
 * \param buf Buffer storing the big endian value
 */
uint32_t 
get_big_endian_32(char *buf) {
	return (uint32_t)(buf[0] >> 24 | buf[1] >> 16 | buf[2] >> 8 | buf[3]);
}

/*!
 * Store 64 bit value in network byte order in buffer
 *
 * \param buf Buffer to store the value in
 * \param val 64 bit value to be converted to big endian
 */
void
set_big_endian_64(uint8_t *buf, uint64_t val)
{
	set_big_endian_32(buf, val >> 32);
	set_big_endian_32(buf + 4, (uint32_t)val);
}

/*!
 * Convert 64 bit value stored in buffer to little endian
 *
 * \param buf Buffer storing the big endian value
 */
uint64_t
get_big_endian_64(char *buf) {
	uint64_t val;

	val = (get_big_endian_32(buf) << 32) | get_big_endian_32(buf+4);
	return val;
}
