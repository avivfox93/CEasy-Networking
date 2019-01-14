/*
 * json.c
 *
 *  Created on: 13 Jan 2019
 *      Author: aviv
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "json.h"

#define JSON_MIN_SIZE 3
#define INTEGER_MAX_CHARS 64

JSON* json_init()
{
	JSON* res = calloc(1,sizeof(JSON));
	if(res == NULL)
		return NULL;
	return res;
}

int _json_data_allocation(JSON* json)
{
	if(json->data == NULL)
		json->data = calloc(1,sizeof(char*));
	else
		json->data = realloc(json->data,(json->values)*sizeof(char*));
	if(json->data == NULL)
		return -1;
	return 1;
}

int json_add_int(JSON* json, const char* key, int value)
{
	char num[INTEGER_MAX_CHARS] = {0};
	if(!_json_data_allocation(json))
		return -1;
	sprintf(num,"%d",value);
	json->data[json->values] = calloc(strlen("\"\":") + strlen(key) + strlen(num),1);
	if(json->data[json->values] == NULL)
			return -1;
	sprintf(json->data[json->values++],"\"%s\":%d",key,value);
	return 1;
}

int json_add_string(JSON* json, const char* key, const char* value)
{
	if(!_json_data_allocation(json))
		return -1;
	json->data[json->values] = calloc(strlen("\"\":\"\"") + strlen(key) + strlen(value),1);
	if(json->data[json->values] == NULL)
		return -1;
	sprintf(json->data[json->values++],"\"%s\":\"%s\"",key,value);
	return 1;
}

int json_add_string_array(JSON* json, const char* key, const char** values, size_t num)
{
	int i;
	if(!_json_data_allocation(json))
		return -1;
	json->data[json->values] = calloc(strlen("\"\":[]") + strlen(key),1);
	if(json->data[json->values] == NULL) return -1;
	sprintf(json->data[json->values],"\"%s\":[",key);
	for(i = 0 ; i < num && values[i] != NULL; i++)
	{
		json->data[json->values] = realloc(json->data[json->values],strlen(json->data[json->values]) + strlen(values[i]) + strlen("\"\","));
		if(json->data[json->values] == NULL)
			return -1;
		sprintf(json->data[json->values],"%s\"%s\",",json->data[json->values],values[i]);
	}
	json->data[json->values][strlen(json->data[json->values]) -1] = ']';
	json->values++;
	return 1;
}

int json_add_int_array(JSON* json, const char* key, const int* values, size_t num)
{
	int i;
	char number[INTEGER_MAX_CHARS] = {0};
	if(!_json_data_allocation(json))
		return -1;
	json->data[json->values] = calloc(strlen("\"\":[]") + strlen(key),1);
	if(json->data[json->values] == NULL) return -1;
	sprintf(json->data[json->values],"\"%s\":[",key);
	for(i = 0 ; i < num ; i++)
	{
		sprintf(number,"%d",values[i]);
		json->data[json->values] = realloc(json->data[json->values],strlen(json->data[json->values]) + strlen(number) + strlen(","));
		if(json->data[json->values] == NULL)
			return -1;
		sprintf(json->data[json->values],"%s%s",json->data[json->values],number);
	}
	json->data[json->values][strlen(json->data[json->values] -1)] = ']';
	json->values++;
	return 1;
}

char* json_get_string(JSON* json, const char* key)
{
	int i,j;
	char *ch,*out;
	for(i = 0 ; i < json->values ; i++)
	{
		if(strstr(json->data[i],key) == json->data[i])
		{
			ch = strchr(json->data[i],':');
			if(*(++ch) != '"')
				return NULL;
			for(j = 0 ; ch[j] != '"' ; j++);
			out = calloc(j + 1,1);
			memcpy(out,ch,j);
			return out;
		}
	}
	return NULL;
}

int* json_get_int(JSON* json, const char* key)
{
	int i,j;
	char *ch,*out;
	for(i = 0 ; i < json->values ; i++)
	{
		if(strstr(json->data[i],key) == json->data[i])
		{
			ch = strchr(json->data[i],':');
			if(*(++ch) != '"')
				return NULL;
			for(j = 0 ; ch[j] != '"' ; j++);
			out = calloc(j + 1,1);
			memcpy(out,ch,j);
			return out;
		}
	}
	return NULL;
}

char* json_to_string(JSON* json)
{
	int i;
	char* res = NULL;
	for(i = 0 ; i < json->values ; i++)
	{
		if(res == NULL)
		{
			res = calloc(strlen(json->data[i]) + 3,1);
			*res = '{';
		}

		else
			res = realloc(res,strlen(res) + strlen(json->data[i]));
		if(res == NULL)
			return NULL;
		sprintf(res,"%s%s,",res,json->data[i]);
	}
	if(res == NULL) return res;
	sprintf(res + strlen(res)-1,"}");
	return res;
}

void json_free(JSON* json)
{
	int i;
	for(i = 0 ; i < json->values ; i++)
		free(json->data[i]);
	free(json->data);
}
