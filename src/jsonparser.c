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

#include "json_internal.h"

#ifdef __cplusplus
extern "C" {
#endif


JSON_PARSER_API 
Object* json_parse(const char* text)
{
	if(!(*text) || text == NULL)
		return NULL;	

	Object* retval;
	json_parser_t* parser;
 	
	parser = json_parser_create(text);
	
	if(!parser)
		return NULL;

	retval = json_internal_parse(parser);
	
	json_parser_destroy(parser);
	
	return retval;
}


JSON_PARSER_API
void json_last_error(json_error_t* error) 
{
	(void *)error;
}

#ifdef __cplusplus
}
#endif
