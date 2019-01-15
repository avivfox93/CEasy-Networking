/*
 * restful_api.c
 *
 *  Created on: 13 Jan 2019
 *      Author: aviv
 */

#include "restful_api.h"

int server_init(SERVER* server, unsigned int port)
{
	struct sockaddr_in serv_addr;
	memset(server,0,sizeof(SERVER));
	server->socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server < 0)
		return -1;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	server->port = port;
	server->addr = serv_addr;
	return bind(server->socket,(struct sockaddr*)&(server->addr),sizeof(server->addr));
}

void* _request_handler(void* ptr)
{
	char buffer[BUFFER_SIZE];
	int clientsocket,clientlen,i;
	request_type_t type;
	ssize_t nbytes_read;
	struct sockaddr clientaddr;
	client_t client;
	SERVER* server = (SERVER*) ptr;
	while(1)
	{
		memset(buffer,0,BUFFER_SIZE);
		clientlen = sizeof(sizeof(server->addr));
		clientsocket = accept(server->socket,(struct sockaddr*)&(server->addr),(socklen_t*)&clientlen);
		printf("\n\nwalla recived!!!\n");
		while((nbytes_read = read(clientsocket, buffer, BUFFER_SIZE)) > 0)
		{
			client.socket = clientsocket;
			client.address = clientaddr.sa_data;
			for(i = 0 ; i < server->num_of_routes ; i++)
			{
				server->routes[i]->callback((header_t*)"walla",(body_t*)"walla",client);
			}
			for(i = 0 ; i < BUFFER_SIZE ; i++)
				putchar(buffer[i]);
			send(clientsocket,buffer,strlen(buffer),0);
			type = get_header_and_body(buffer,nbytes_read,NULL,NULL);
			if(buffer[nbytes_read - 1] == '\n')
				break;
			sched_yield();
		}
		close(clientsocket);
	}
}

int server_listen(SERVER* server, int max_conn)
{
	int reuseaddr = 1;
	if(listen(server->socket,max_conn) == -1)
		return -1;
	setsockopt(server->socket,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr));
	pthread_t thread = server->thread;
	pthread_create(&thread,NULL,_request_handler,(void*)server);
//	_request_handler((void*)server);
	return 1;
}

void server_get(SERVER* server, route_t* route)
{
	if(server->get_routes == NULL || server->num_of_get_routes == 0)
		server->get_routes = calloc(1,sizeof(route_t*));
	else
		server->get_routes = realloc(server->get_routes,(server->num_of_get_routes + 1)*sizeof(route_t));
	server->get_routes[server->num_of_get_routes++] = route;
}

void server_post(SERVER* server, route_t* route)
{
	if(server->post_routes == NULL || server->num_of_post_routes == 0)
		server->post_routes = calloc(1,sizeof(route_t*));
	else
		server->post_routes = realloc(server->post_routes,(server->num_of_post_routes + 1)*sizeof(route_t));
	server->post_routes[server->num_of_post_routes++] = route;
}

void server_set(SERVER* server, route_t* route)
{
	if(server->routes == NULL || server->num_of_routes == 0)
		server->routes = calloc(1,sizeof(route_t*));
	else
		server->routes = realloc(server->routes,(server->num_of_routes + 1)*sizeof(route_t));
	server->routes[server->num_of_routes++] = route;
}

request_type_t get_header_and_body(const char* buffer, int buffer_size, body_t* body, header_t* header)
{
	request_type_t type;
	int i;
	char *a,*b;
	type = get_request_type(buffer);
	b= strchr(buffer,'/');
	a = b + 1;
	while(*b != '?' && *b != ' ')b++;
	printf("\nserver recv: \n");
	while(a < b)putchar(*(a++));
	for(i = 0 ; i < buffer_size ; i++)
	{

	}
	return type;
}

char* get_param(const body_t* body, const char* key)
{
	return NULL;
}

char* get_value_header(const header_t* header, const char* key)
{
	return NULL;
}

void server_free(SERVER* server)
{
	if(server->num_of_routes > 0)
		free(server->routes);
	if(server->num_of_get_routes > 0)
		free(server->get_routes);
	if(server->num_of_post_routes > 0)
		free(server->post_routes);
	pthread_cancel((server->thread));
	close(server->socket);
}
