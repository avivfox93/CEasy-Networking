/*
 * json.h
 *
 *  Created on: 13 Jan 2019
 *      Author: aviv
 */

#ifndef UTILS_JSON_H_
#define UTILS_JSON_H_

typedef struct
{
	char** 	data;
	int 	values;
} JSON;

int json_add_int(JSON* json, const char* key, int value);

int json_add_string(JSON* json, const char* key, const char* value);

int json_add_string_array(JSON* json, const char* key, const char** values, size_t num);

int json_add_int_array(JSON* json, const char* key, const int* values, size_t num);

char* json_get_string(JSON* json, const char* key);

int json_get_int(JSON* json, const char* key, int* out);

char* json_to_string(JSON* json);

void json_free(JSON* json);

#endif /* UTILS_JSON_H_ */
