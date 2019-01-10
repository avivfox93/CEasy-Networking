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

int main()
{
	char* message;
	char buff[2048] = {0};
	SSL_CONNECTION* ssl = calloc(1,sizeof(SSL_CONNECTION));
	ssl_init(NULL);
	ssl->address = calloc(64,1);
	strcpy(ssl->address,"https://www.google.com");

	init_connection(ssl);

	message = get_request("google.com","/?some=params&some=more","");

	write_to_ssl_socket(ssl,message,strlen(message),buff,2048);

	printf("wallak: %s",buff);

	free(message);
	free_ssl_connection(ssl);
	return 0;
}
