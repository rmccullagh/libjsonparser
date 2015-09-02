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
#include "json_internal.h"

static bool is_control_char(int);
static void parser_next(json_parser_t* self);

json_parser_t* 
json_parser_new(const char* text)
{
	json_parser_t* self;
	size_t text_length;
	
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

	memcpy(self->text, source, self->len);
	
	self->text[self->len] = '\0';
	self->pos = 0;
	self->line = 1;
	self->look = -1;
	self->status = 0;
	
	return self;
}

Object* 
json_internal_parse(json_parser_t* self)
{
	Object* retval = NULL;
	
	return retval;
}











static bool is_control_char(int c)
{
	if(c == 0x7f)
		return true;
	return (c >= 0x00 && c <= 0x1f);
}



