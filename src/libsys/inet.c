/*
 * inet.c - Internet address manipulation routines.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "inet.h"

#include <sys/socket.h>
#include <arpa/inet.h>

/**
 * is_valid_ip_address - tries to understand is given
 * parameter ip address or not.
 *
 * Return: true or false.
 */
bool is_valid_ip_address(const char *ip_addr)
{
	struct sockaddr_in sa;

	return inet_pton(AF_INET, ip_addr, &(sa.sin_addr)) != 0;
}
