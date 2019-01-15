/*
 * network_utils.c
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#include "network_utils.h"

const char* _REQUEST_TYPES[] =
{
		"POST",
		"GET",
		"PUT",
		"PUSH",
		"DELETE",
		"HEAD",
		"CONNECT",
		"OPTIONS",
		"TRACE",
		"PATCH",
		""
};

/*
 * Template Request
 */
const char* REQUEST_TEMPLATE =
		"%s %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Content-type: %s\r\n"
		"Content-length: %d\r\n";

/*
 * GET Request Template
 */
const char* GET_REQUEST =
		     "GET %s HTTP/1.1\r\n"
		     "Host: %s\r\n"
		     "Content-type: %s\r\n"
		     "Content-length: %d\r\n"
		     "%s\r\n"
		     "\r\n"
		     "%s\r\n";

/*
 * POST Request Template
 */
const char* POST_REQUEST =
		"POST %s? HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Content-Type: %s\r\n"
		"Content-Length: %d\r\n"
		"%s\r\n"
		"\r\n"
		"%s\r\n";


char* get_request(const char* host, const char* path, const char* msg, const char* header, content_type_t type)
{
	char* res = calloc(strlen(host) + strlen(path) + strlen(msg) + strlen(header) + strlen(GET_REQUEST) + strlen(get_content_type_string(type)),1);
	if(res == NULL) return NULL;
	sprintf(res,GET_REQUEST,path,host,get_content_type_string(type),strlen(msg),header,msg);
	return res;
}


char* post_request(const char* host, const char* path, const char* msg, const char* header, content_type_t type)
{
	char* res = calloc(strlen(host) + strlen(path) + strlen(msg) + strlen(header) + strlen(POST_REQUEST) + strlen(get_content_type_string(type)),1);
	if(res == NULL) return NULL;
	sprintf(res,POST_REQUEST,path,host,get_content_type_string(type),strlen(msg),header,msg);
	return res;
}

const char* get_content_type_string(content_type_t type)
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

char* request_to_string(const request_t* request)
{
	int i;
	char *dataout,data[REQUEST_MAX_LENGTH];
	if(request == NULL)
		return NULL;
	sprintf(data,REQUEST_TEMPLATE,
				 request_type_to_string(request->type),request->path,
				 request->host,
				 get_content_type_string(request->header->content_type),
				 strlen(request->header->content));
	for(i = 0 ; i < request->header->num_of_keys ; i++)
		sprintf(data + strlen(data),"%s : %s\r\n",request->header->key[i],request->header->value[i]);
	sprintf((char*)(data + strlen(data)), "\r\n%s\r\n",request->header->content);
	dataout = calloc(strlen(data),1);
	if(dataout == NULL)
		return NULL;
	strcpy(dataout,data);
	return dataout;
}

request_type_t get_request_type(const char* buffer)
{
	if(strstr(buffer,"PUSH") == buffer)
		return PUSH;
	else if(strstr(buffer,"POST") == buffer)
		return POST;
	else if(strstr(buffer,"GET") == buffer)
		return GET;
	else if(strstr(buffer,"PUT") == buffer)
		return PUT;
	else if(strstr(buffer,"DELETE") == buffer)
		return DELETE_r;
	else if(strstr(buffer,"HEAD") == buffer)
		return HEAD;
	else if(strstr(buffer,"CONNECT") == buffer)
		return CONNECT;
	else if(strstr(buffer,"OPTIONS") == buffer)
		return OPTIONS;
	else if(strstr(buffer,"TRACE") == buffer)
		return TRACE;
	else if(strstr(buffer,"PATCH") == buffer)
		return PATCH;
	return UNDEFINED;
}

const char* request_type_to_string(request_type_t type)
{
	return _REQUEST_TYPES[type];
}

void add_param_to_header(header_t* header, const char* key, const char* value)
{
	if(header->num_of_keys == 0)
	{
		header->key = malloc(sizeof(char*));
		header->value = malloc(sizeof(char*));
	}
	else
	{
		header->key = realloc(header->key,sizeof(char*)*(1 + header->num_of_keys));
		header->value = realloc(header->key,sizeof(char*)*(1 + header->num_of_keys));
	}
	header->key[header->num_of_keys] = malloc(strlen(key));
	strcpy(header->key[header->num_of_keys],key);
	header->value[header->num_of_keys] = malloc(strlen(value));
	strcpy(header->value[header->num_of_keys],value);
	header->num_of_keys++;
}

