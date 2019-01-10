/*
 * network_utils.h
 *
 *  Created on: 10 Jan 2019
 *      Author: aviv
 */

#ifndef UTILS_NETWORK_UTILS_H_
#define UTILS_NETWORK_UTILS_H_

typedef enum
{
	application_x_www_form_urlencoded,
	text_plain,
	application_json
}Content_Type;

const char* GET_REQUEST;
const char* POST_REQUEST;
/*
 * Return GET Request with Parameters
 */
char* get_request(const char* host, const char* path, const char* msg, Content_Type type);
/*
 * Return POST Request with Parameters
 */
char* post_request(const char* host, const char* path, const char* msg, Content_Type type);
/*
 * Return Content Type as string
 */
const char* get_content_type_string(Content_Type type);

#endif /* UTILS_NETWORK_UTILS_H_ */
