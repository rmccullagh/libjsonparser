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

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "json_internal.h"
#include "json_value.h"

JSON_PARSER_PRIVATE json_parser_t* 
json_parser_create(const char* text)
{
	json_parser_t* self;
	
	self = calloc(1, sizeof(json_parser_t));
	
	if(!self)
		return NULL;
	
	self->len = strlen(text);
	self->text = malloc(self->len + 1);
	
	if(!self->text) 
	{
		free(self);
		return NULL;
	}

	memcpy(self->text, text, self->len);
	
	self->text[self->len] = '\0';
	self->pos = 0;
	self->line = 1;
	self->look = EOF;
	self->status = 0;
	self->error_message = "";

	return self;
}

JSON_PARSER_PRIVATE void 
json_parser_destroy(json_parser_t* self)
{
	free(self->text);
	free(self);
	json_value_reset();
}

JSON_PARSER_PRIVATE Object* 
json_internal_parse(json_parser_t* self)
{
	return json_parse_value(self);
}
