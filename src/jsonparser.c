/**
 * Copyright (c) 2015 Ryan McCullagh <me@ryanmccullagh.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <jsonparser.h>
#include <object.h>
#include <mutablestring.h>
#include <string.h>
#include <stdlib.h>

#include "json_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The global structure in this compiliation unit
 */
static json_parser_t* parser = NULL;
static json_error_t error;

static void 
json_set_last_error(json_error_t* error) 
{
	size_t msg_len = strlen(parser->error_message);
	
	if(msg_len) {
		error->message = malloc(msg_len + 1);
		memcpy(error->message, parser->error_message, msg_len + 1);
	} else {
		error->message = NULL;
	}
	error->column = parser->pos;
	error->line = parser->line;
	error->token = parser->look;
}

JSON_PARSER_API 
Object* json_parse(const char* text)
{
	if(text == NULL)
		return NULL;	

	Object* retval;
 	
	parser = json_parser_create(text);
	
	if(!parser)
		return NULL;

	retval = json_internal_parse(parser);

	if(retval)
	{
		if(parser->pos < parser->len)
		{
			parser->error_message = "unexpected token (finished parsing stream early)";
			
			json_set_last_error(&error);

			json_parser_destroy(parser);
			
			objectDestroy(retval);

			return NULL;		
		}
	}

	json_set_last_error(&error);

	json_parser_destroy(parser);

	parser = NULL;

	return retval;
}

JSON_PARSER_API
void json_last_error(json_error_t* _error) 
{

	size_t msg_len = strlen(error.message);
	
	if(msg_len) {
		_error->message = malloc(msg_len + 1);
		memcpy(_error->message, error.message, msg_len + 1);
		free(error.message);
	} else {
		_error->message = NULL;
	}
	
	error.message = NULL;
	
	_error->column = error.column;
	_error->line = error.line;
	_error->token = error.token;

	error.column = 0;
	error.line = 0;
	error.token = 'F';
}

#ifdef __cplusplus
}
#endif
