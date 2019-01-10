/*
 * network_utils.h
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#ifndef UTILS_NETWORK_UTILS_H_
#define UTILS_NETWORK_UTILS_H_

const char* GET_REQUEST;
const char* POST_REQUEST;

char* get_request(const char* host, const char* path, const char* msg);
char* post_request(const char* host, const char* path, const char* msg);

#endif /* UTILS_NETWORK_UTILS_H_ */
