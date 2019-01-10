/*
 * ssl_connect.h
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#ifndef UTILS_SSL_CONNECT_H_
#define UTILS_SSL_CONNECT_H_

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/evp.h>

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


typedef struct
{
	char*		address;
	BIO*		outbio;
	X509*		cert;
	X509_NAME*	certname;
	SSL_CTX* 	ctx;
	SSL* 		ssl;
	int 		socket;
} SSL_CONNECTION;

int create_ssl_socket(char[], BIO *);
int init_connection(SSL_CONNECTION* conn);
int ssl_init(BIO** outbio);
int create_ssl_socket(char url_str[], BIO *out);
void free_ssl_connection(SSL_CONNECTION* conn);
int write_to_ssl_socket(SSL_CONNECTION* conn, const void* data, int size, void* outbuff, int buffsize);

#endif /* UTILS_SSL_CONNECT_H_ */
