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

#define ADDRESS "postman-echo.com"

int main()
{
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

	char* str = json_to_string(json);
	printf("JSON: %d\n%s",json->values,str);

	init_connection(ssl);

	message = get_request(ADDRESS,"/get?some=params","",text_plain);
	printf("\nsending: \n%s\n\n",message);

	len = write_to_ssl_socket(ssl,message,strlen(message),buff,2048);
	free(message);
	printf("recieved %d: \n%s",len,buff);

	memset(buff,0,2048);
	message = post_request(ADDRESS,"/post",str,application_json);
	printf("\nsending: \n%s\n\n",message);

	len = write_to_ssl_socket(ssl,message,strlen(message),buff,2048);
	free(message);
	printf("recieved %d: \n%s",len,buff);

	free(str);
	json_free(json);
	free_ssl_connection(ssl);
	return 0;
}
