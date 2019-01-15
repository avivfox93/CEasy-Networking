/*
 * network_utils.h
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#ifndef UTILS_NETWORK_UTILS_H_
#define UTILS_NETWORK_UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define REQUEST_MAX_LENGTH 1024*256 // 256KB

typedef enum
{
	POST,
	GET,
	PUT,
	PUSH,
	DELETE_r,
	HEAD,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH,
	UNDEFINED
}request_type_t;

typedef enum
{
	application_x_www_form_urlencoded,
	text_plain,
	application_json
}content_type_t;

typedef struct
{
	int status;
	char** key;
	char** value;
	int num_of_keys;
	char* content;
	content_type_t content_type;
}header_t;

typedef struct
{
	request_type_t type;
	header_t* header;
	const char* host;
	const char* path;

}request_t;

const char* GET_REQUEST;
const char* POST_REQUEST;
/*
 * Return GET Request with Parameters
 */
char* get_request(const char* host, const char* path, const char* msg, const char* header, content_type_t type);
/*
 * Return POST Request with Parameters
 */
char* post_request(const char* host, const char* path, const char* msg, const char* header, content_type_t type);
/*
 * Return Content Type as string
 */
const char* get_content_type_string(content_type_t type);
/*
 * Returns request_t as string
 */
char* request_to_string(const request_t* request);

request_type_t get_request_type(const char* buffer);

const char* request_type_to_string(request_type_t type);

void add_param_to_header(header_t* header, const char* key, const char* value);

#endif /* UTILS_NETWORK_UTILS_H_ */
