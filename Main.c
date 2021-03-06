/*
 * Main.c
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */


#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#include "utils/network_utils.h"
#include "utils/ssl_connect.h"
#include "utils/json.h"
#include "utils/restful_api.h"

#define ADDRESS "postman-echo.com"

int main()
{
	request_t request;
	header_t header;

	SERVER server;
	const char* walla[] = {"ehad","shtaim","shalosh"};
	JSON* json;
	int len;
	char* message;
	char buff[2048] = {0};
	SSL_CONNECTION* ssl = calloc(1,sizeof(SSL_CONNECTION));
	ssl_init(NULL);
	strcpy(ssl->address,"https://"ADDRESS);

	json = calloc(1,sizeof(JSON));
	json_add_string(json,"stam","String");
	json_add_int(json,"justAnum",55);
	json_add_string_array(json,"Sarray",walla,3);

	char* str = json_to_string(json);
	printf("JSON: %d\n%s",json->values,str);

	init_connection(ssl);
	memset(&request,0,sizeof(request_t));
	memset(&header,0,sizeof(header_t));

	request.header = &header;
	request.host = ADDRESS;
	request.type = POST;
	request.path = "/post";
	header.content_type = text_plain;
	header.content = "some data";
	add_param_to_header(&header,"Header","Param");

	message = request_to_string(&request);
	printf("\nwalla send:\n%s\n\n",message);

	len = write_to_ssl_socket(ssl,message,strlen(message),buff,2048);
	free(message);
	printf("recieved %d: \n%s",len,buff);

	memset(buff,0,2048);
	message = get_request(ADDRESS,"/get?some=params","","Authorization : BEARER 12345678",text_plain);
	printf("\nsending: \n%s\n\n",message);

	len = write_to_ssl_socket(ssl,message,strlen(message),buff,2048);
	free(message);
	printf("recieved %d: \n%s",len,buff);

	memset(buff,0,2048);
	message = post_request(ADDRESS,"/post",str,"Stam : Header",application_json);
	printf("\nsending: \n%s\n\n",message);

	len = write_to_ssl_socket(ssl,message,strlen(message),buff,2048);
	free(message);
	printf("recieved %d: \n%s",len,buff);

	free(str);
	json_free(json);
	free_ssl_connection(ssl);


	printf("\nserver: %d\n",server_init(&server,3322));
	server_listen(&server,5);
	while(getchar() != 'e');
	server_free(&server);
	return 0;
}
