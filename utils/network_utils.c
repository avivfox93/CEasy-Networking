/*
 * network_utils.c
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#include "network_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char* GET_REQUEST =
		     "GET %s HTTP/1.0\r\n"
		     "Host: %s\r\n"
		     "Content-type: application/x-www-form-urlencoded\r\n"
		     "Content-length: %d\r\n\r\n"
		     "%s\r\n";

const char* POST_REQUEST =
		"POST /path HTTP/1.0\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: %d\r\n"
		"\r\n";

char* get_request(const char* host, const char* path, const char* msg)
{
	char* res = calloc(strlen(host) + strlen(path) + strlen(msg) + strlen(GET_REQUEST) + 1,1);
	if(res == NULL) return NULL;
	sprintf(res,GET_REQUEST,path,host,strlen(msg),msg);
	return res;
}

char* post_request(const char* host, const char* path, const char* msg)
{
	char* res = calloc(strlen(host) + strlen(path) + strlen(msg) + strlen(GET_REQUEST),1);
	if(res == NULL) return NULL;
	sprintf(res,GET_REQUEST,path,host,strlen(msg),msg);
	return res;
}


