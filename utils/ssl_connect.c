#include "ssl_connect.h"

int init_connection(SSL_CONNECTION* conn)
{
	if(conn->outbio == NULL)
		conn->outbio = BIO_new_fp(stdout, BIO_NOCLOSE);
	const SSL_METHOD* method = SSLv23_client_method();
	conn->ctx = SSL_CTX_new(method);
	if(conn->ctx == NULL) return -1;
	SSL_CTX_set_options(conn->ctx, SSL_OP_NO_SSLv2);
	conn->ssl = SSL_new(conn->ctx);
	conn->socket = create_ssl_socket(conn->address,conn->outbio);
	SSL_set_fd(conn->ssl, conn->socket);
	return SSL_connect(conn->ssl);
}

int ssl_init(BIO** outbio)
{
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	if(outbio != NULL)
		*outbio  = BIO_new_fp(stdout, BIO_NOCLOSE);
	return SSL_library_init();
}
//from http://fm4dd.com/openssl/sslconnect.htm
/* ---------------------------------------------------------- *
 * create_socket() creates the socket & TCP-connect to server *
 * ---------------------------------------------------------- */
int create_ssl_socket(char url_str[], BIO *out)
{
  int			sockfd;
  char 			hostname[256] = "";
  char			portnum[6] = "443";
  char			proto[6] = "";
  char*			tmp_ptr = NULL;
  int			port;
  struct hostent* host;
  struct sockaddr_in dest_addr;

  /* ---------------------------------------------------------- *
   * Remove the final / from url_str, if there is one           *
   * ---------------------------------------------------------- */
  if(url_str[strlen(url_str)] == '/')
    url_str[strlen(url_str)] = '\0';

  /* ---------------------------------------------------------- *
   * the first : ends the protocol string, i.e. http            *
   * ---------------------------------------------------------- */
  strncpy(proto, url_str, (strchr(url_str, ':')-url_str));

  /* ---------------------------------------------------------- *
   * the hostname starts after the "://" part                   *
   * ---------------------------------------------------------- */
  strncpy(hostname, strstr(url_str, "://")+3, sizeof(hostname));

  /* ---------------------------------------------------------- *
   * if the hostname contains a colon :, we got a port number   *
   * ---------------------------------------------------------- */
  if(strchr(hostname, ':')) {
    tmp_ptr = strchr(hostname, ':');
    /* the last : starts the port number, if avail, i.e. 8443 */
    strncpy(portnum, tmp_ptr+1,  sizeof(portnum));
    *tmp_ptr = '\0';
  }

  port = atoi(portnum);

  if ( (host = gethostbyname(hostname)) == NULL ) {
    BIO_printf(out, "Error: Cannot resolve hostname %s.\n",  hostname);
    abort();
  }

  /* ---------------------------------------------------------- *
   * create the basic TCP socket                                *
   * ---------------------------------------------------------- */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  dest_addr.sin_family=AF_INET;
  dest_addr.sin_port=htons(port);
  dest_addr.sin_addr.s_addr = *(long*)(host->h_addr);

  /* ---------------------------------------------------------- *
   * Zeroing the rest of the struct                             *
   * ---------------------------------------------------------- */
  memset(&(dest_addr.sin_zero), '\0', 8);

  tmp_ptr = inet_ntoa(dest_addr.sin_addr);

  /* ---------------------------------------------------------- *
   * Try to make the host connect here                          *
   * ---------------------------------------------------------- */
  if ( connect(sockfd, (struct sockaddr *) &dest_addr,
                              sizeof(struct sockaddr)) == -1 ) {
    BIO_printf(out, "Error: Cannot connect to host %s [%s] on port %d.\n",
             hostname, tmp_ptr, port);
  }

  return sockfd;
}

/*
 * Release SSL_CONNECTION struct
 */
void free_ssl_connection(SSL_CONNECTION* conn)
{
	SSL_free(conn->ssl);
	close(conn->socket);
	X509_free(conn->cert);
	SSL_CTX_free(conn->ctx);
}

/*
 * Write data to SSL_CONNECTION
 */
int write_to_ssl_socket(SSL_CONNECTION* conn, const void* data, size_t size, void* outbuff, size_t buffsize)
{
	int len;
	len = SSL_write(conn->ssl,data,size);
	if(outbuff == NULL || buffsize == 0) return len;
	SSL_read(conn->ssl,outbuff,buffsize);
	return len;
}


