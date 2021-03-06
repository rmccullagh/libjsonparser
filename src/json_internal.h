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

#ifndef __JSON_INTERNAL_H__
#define __JSON_INTERNAL_H__

#include <stddef.h>
#include <object.h>
#include "jsonparser.h"

typedef struct json_parser_t {
	char* text;
	size_t pos;
	size_t len;
	size_t line;
	char look;
	int status;
	const char* error_message;
} json_parser_t;


JSON_PARSER_PRIVATE
json_parser_t* json_parser_create(const char*);

JSON_PARSER_PRIVATE
void json_parser_destroy(json_parser_t*);

JSON_PARSER_PRIVATE
Object* json_internal_parse(json_parser_t*);

#endif /* __JSON_INTERNAL_H__ */
