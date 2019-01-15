/*
 * restful_api.h
 *
 *  Created on: 13 Jan 2019
 *      Author: aviv
 */

#ifndef UTILS_RESTFUL_API_H_
#define UTILS_RESTFUL_API_H_

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sched.h>
#include "network_utils.h"

#define BUFFER_SIZE 2048

typedef struct
{
	const char* address;
	int socket;
}client_t;

//typedef struct
//{
//	const char** keys;
//	const char** values;
//	int num_of_values;
//}header_t;

typedef struct
{
	const char** keys;
	const char** values;
	int num_of_values;
}body_t;

typedef struct
{
	const char* route;
	const void (*callback)(const header_t* header, const body_t* body, const client_t client);
}route_t;

typedef struct
{
	int socket;
	unsigned int port;
	route_t** routes;
	route_t** post_routes;
	route_t** get_routes;
	int num_of_routes;
	int num_of_post_routes;
	int num_of_get_routes;
	const route_t* not_found;
	struct sockaddr_in addr;
	pthread_t thread;
}SERVER;

int server_init(SERVER* server, unsigned int port);

int server_listen(SERVER* server, int max_conn);

void server_get(SERVER* server, route_t* route);

void server_post(SERVER* server, route_t* route);

void server_set(SERVER* server, route_t* route);

request_type_t get_header_and_body(const char* buffer, int buffer_size, body_t* body, header_t* header);

char* get_param(const body_t* body, const char* key);

char* get_value_header(const header_t* header, const char* key);

request_type_t get_request_type(const char* buffer);

void server_free(SERVER* server);

#endif /* UTILS_RESTFUL_API_H_ */
