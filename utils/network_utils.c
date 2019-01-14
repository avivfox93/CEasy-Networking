/*
 * network_utils.c
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#include "network_utils.h"

/*
 * GET Request Template
 */
const char* GET_REQUEST =
		     "GET %s HTTP/1.1\r\n"
		     "Host: %s\r\n"
		     "Content-type: %s\r\n"
		     "Content-length: %d\r\n\r\n"
		     "%s\r\n";

/*
 * POST Request Template
 */
const char* POST_REQUEST =
		"POST %s? HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Content-Type: %s\r\n"
		"Content-Length: %d\r\n\r\n"
		"%s\r\n";


char* get_request(const char* host, const char* path, const char* msg, Content_Type type)
{
	char* res = calloc(strlen(host) + strlen(path) + strlen(msg) + strlen(GET_REQUEST) + strlen(get_content_type_string(type)),1);
	if(res == NULL) return NULL;
	sprintf(res,GET_REQUEST,path,host,get_content_type_string(type),strlen(msg),msg);
	return res;
}


char* post_request(const char* host, const char* path, const char* msg, Content_Type type)
{
	char* res = calloc(strlen(host) + strlen(path) + strlen(msg) + strlen(POST_REQUEST) + strlen(get_content_type_string(type)),1);
	if(res == NULL) return NULL;
	sprintf(res,POST_REQUEST,path,host,get_content_type_string(type),strlen(msg),msg);
	return res;
}

const char* get_content_type_string(Content_Type type)
{
	switch(type)
	{
	case text_plain:
		return "text/plain";
	case application_x_www_form_urlencoded:
		return "application/x-www-form-urlencoded";
	case application_json:
		return "application/json-rpc; charset=UTF-8";
	default:
		return "text/plain";
	}
}


