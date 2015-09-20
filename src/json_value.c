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

#include <stdbool.h>
#include <ctype.h>
#include <mutablestring.h>
#include <object.h>
#include <stdlib.h>
#include "json_internal.h"
#include "json_value.h"

static int first_pass = 0;

JSON_PARSER_PRIVATE
void json_value_reset(void)
{
	first_pass = 0;
}

static void 
parser_next(json_parser_t*);
static bool
is_start_of_int(json_parser_t*);
static bool
is_start_of_string(json_parser_t*);
static void 
set_error(json_parser_t*, const char*);

static void 
skip_white(json_parser_t* self)
{
	
	while(self->look == '\r' || self->look == '\n' || 
			self->look == '\t' || self->look == ' ') 
	{
		if(self->look == '\n') 
		{
			self->line++;
		}
		parser_next(self);
	}
}

static void 
expected(json_parser_t* self)
{
	switch(self->look)
	{
		case '\0':
			set_error(self, "unexpected end of file");
		break;
		default:
			set_error(self, "unexpected token");
		break;
	}
}

static bool 
match(json_parser_t* self, const char c)
{
	bool retval;

	if(self->look == c)
	{
		parser_next(self);
		retval = true;	
	}
	else
	{
		expected(self);
		retval = false;	
	}

	return retval;
}


static void 
parser_next(json_parser_t* self)
{
	if(self->pos >= self->len) 
	{
		self->status = EOF;
		self->look = '\0';
	}	
	else 
	{
		self->look = self->text[self->pos++];
	}
}

static bool
json_is_valid_first_char(json_parser_t* self)
{
	if(self->look != '{' && self->look != '[') 
	{
		self->error_message = "expecting '{' or '['";
		return false;
	}
	return true;
}

static bool
json_value_first_check(json_parser_t* self)
{
	first_pass = 1;

	parser_next(self);
	skip_white(self);
	
	if(!json_is_valid_first_char(self)) 
	{
		return false;
	}

	return true;
}

static bool
is_start_of_int(json_parser_t* self)
{
	return self->look == '-' || isdigit((int)self->look);
}

static bool
is_start_of_string(json_parser_t* self)
{	
	return self->look == '"';
}

static bool
is_start_of_object(json_parser_t* self)
{
	return self->look == '{';
}

static bool
is_start_of_array(json_parser_t* self)
{
	return self->look == '[';
}

static bool
is_null_or_bool(json_parser_t* self)
{
	char c = self->look;
	return c == 't' || c == 'f' || c == 'n';
}

static void
set_error(json_parser_t* self, const char* message)
{
	self->error_message = message;
}

static double 
parse_number(json_parser_t* self, bool* success)
{
	bool is_neg = false;
	int first = 0;
	double dval;
	MutableString* buffer;
	*success = false;

	if(self->look == '-')
	{
		parser_next(self);
		is_neg = true;
	}
	
	if(!isdigit((int)self->look)) 
	{
		set_error(self, "'-' must be followed by a digit");
		goto done;
	}

	buffer = mutable_string_new(NULL);

	if(!buffer)
	{
		set_error(self, "Out of memory");
		goto done;
	}

	first = self->look - '0';
	mutable_string_append_c(buffer, self->look);
	parser_next(self);

	if(first == 0 && self->look != '.')
	{
		mutable_string_free(buffer);
		set_error(self, 
				"unexpected 0, a number can only be followed by one 0, and that zero must be followed by a '.'");

		goto done;
	}

	while(isdigit((int)self->look))
	{
		mutable_string_append_c(buffer, self->look);
		parser_next(self);
	}

	if(self->look == '.')
	{
		mutable_string_append_c(buffer, self->look);
		parser_next(self);
		
		if(!isdigit((int)self->look))
		{
			set_error(self, 
					"expected a digit after the decimal point");
			mutable_string_free(buffer);
			goto done;
		}

		while(isdigit((int)self->look))
		{
			mutable_string_append_c(buffer, self->look);
			parser_next(self);
		}
	}

	if(self->look == 'e' || self->look == 'E')
	{
		mutable_string_append_c(buffer, self->look);
		parser_next(self);
	
		if(self->look == '+' || self->look == '-') 
		{
			mutable_string_append_c(buffer, self->look);
			parser_next(self);
		}

		if(!isdigit((int)self->look))
		{
			set_error(self,
					"expected the exponent part to be followed by a digit");
			mutable_string_free(buffer);
			goto done;
		}
		
		while(isdigit((int)self->look))
		{
			mutable_string_append_c(buffer, self->look);
			parser_next(self);
		}
	}

	*success = true;

	dval = strtod(buffer->buffer, NULL);

	mutable_string_free(buffer);

	if(dval && is_neg)
		return -dval;
	else
		return dval;

	done:
		return 0;
}

static bool
is_control_char(int c)
{
	if(c == 0x7f)
		return true;
	return c >= 0x00 && c <= 0x1F;

}
static bool 
is_ascii_code_point(int val)
{
	return val >= 0x0000 && val <= 0x007F;
}

/*
 * https://encoding.spec.whatwg.org/#utf-8-encoder
 */
static void
utf8_encode(MutableString* buffer, unsigned long code_point)
{
	int count = 0, offset = 0, temp = 0;

	if(is_ascii_code_point(code_point))
	{
		mutable_string_append_c(buffer, (char)code_point);
	}

	if(code_point >= 0x0080 && code_point <= 0x07FF)
	{
		count = 1;
		offset = 0xC0;
	}
	else if(code_point >= 0x0800 && code_point <= 0xFFFF)
	{		
		count = 2;
		offset = 0XE0;
	}
	else if(code_point >= 0x10000 && code_point <= 0x10FFFF)
	{
		count = 3;
		offset = 0xF0;
	}

	mutable_string_append_c(buffer, 
			(char)((code_point >> (6 * count)) + offset));

	while(count > 0)
	{
		temp = code_point >> (6 * (count - 1));
		mutable_string_append_c(buffer, (char)(0x80 | (temp & 0x3f)));
		count--;
	}
}

static MutableString*
parse_string(json_parser_t* self)
{

#define CASE_ACTION(_char) \
	mutable_string_append_c(str, _char); \
	bytes_read++; \
	parser_next(self); \

	MutableString* str;
	size_t bytes_read;

	str = mutable_string_new_len(2);
	bytes_read = 0;

	if(!str)
		return NULL;

	while(self->look != '\0' && self->look != '"')
	{
		if(is_control_char((int)self->look))
		{
			set_error(self, "unexpected escape character found in string");
			goto fail;
		}
		else if(self->look == '\\')
		{
			bytes_read++;
			if(bytes_read >= self->len)
			{
				set_error(self, "invalid escape sequence in string");
				goto fail;
			}	
			
			parser_next(self);

			switch(self->look)
			{
				case '"':
					CASE_ACTION('"')
				break;
				case '\\':
					CASE_ACTION('\\')
				break;
				case '/':
					CASE_ACTION('/')
				break;
				case 'b':
					CASE_ACTION('\b')
				break;
				case 'f':
					CASE_ACTION('\f')
				break;
				case 'n':
					CASE_ACTION('\n')
				break;
				case 'r':
					CASE_ACTION('\r')
				break;
				case 't':
					CASE_ACTION('\t')
				break;
				case 'u': {
					bytes_read++;
					
					if(self->pos + 4 >= self->len)
					{
						set_error(self, "invalid unicode escape sequence in string");
						goto fail;
					}
					
					parser_next(self);
					
					if(!isxdigit((int)self->look))
					{
						set_error(self, "unicode escape sequences must be \
								followed by hexadecimal digits");
						goto fail;
					}
					
					MutableString* hex_buffer = mutable_string_new_len(5);
					if(!hex_buffer)
					{
						set_error(self, "out of memory");
						goto fail;
					}

					int seq = 0;
		
					for(seq = 0; seq < 4; seq++)
					{	
						if(!isxdigit((int)self->look))
						{	
							set_error(self, "invalid unicode escape sequence");
							mutable_string_free(hex_buffer);
							goto fail;
						}
						mutable_string_append_c(hex_buffer, self->look);
						parser_next(self);
					}

					bytes_read += 4;	
					unsigned long code_point;
					code_point = strtoul(hex_buffer->buffer, NULL, 16);

					MutableString* utf8 = mutable_string_new(NULL);
					utf8_encode(utf8, code_point);
					mutable_string_append(str, utf8->buffer);
					mutable_string_free(hex_buffer);
					mutable_string_free(utf8);
				}
				default:
					set_error(self, "invalid escape sequence");
					goto fail;
				break;	
			}	
		}
		else
		{	
			mutable_string_append_c(str, self->look);
			bytes_read++;
			parser_next(self);
		}
	}

	return str;

	fail:
		mutable_string_free(str);
		return NULL;
}

static Object*
parse_object(json_parser_t* self)
{
	skip_white(self);
	Object* map = newMap(2);

	if(!map)
		return NULL;

	if(self->look == '}')
		return map;

	if(!match(self, '"'))
	{
		objectDestroy(map);
		return NULL;
	}

	MutableString* key = parse_string(self);

	if(!key) 
	{
		objectDestroy(map);
		return NULL;
	}

	if(!match(self, '"'))
	{
		objectDestroy(map);
		mutable_string_free(key);
		return NULL;
	}

	skip_white(self);

	if(!match(self, ':'))
	{
		mutable_string_free(key);
		objectDestroy(map);
		set_error(self, "expecting ':' after object key");
		return NULL;
	}

	skip_white(self);

	Object* value = json_parse_value(self);
	
	if(!value)
	{
		mutable_string_free(key);
		objectDestroy(map);
		return NULL;
	}

	skip_white(self);
	mapInsert(map, key->buffer, value);
	mutable_string_free(key);
	objectDestroy(value);
	skip_white(self);

	while(self->look == ',')
	{	
		if(!match(self, ',')) {
			objectDestroy(map);
			return NULL;
		}

		skip_white(self);

		if(!match(self, '"'))
		{
			objectDestroy(map);
			return NULL;
		}

		key = parse_string(self);
		
		if(!key)
		{
			objectDestroy(map);
			return NULL;
		}

		if(!match(self, '"'))
		{
			mutable_string_free(key);
			objectDestroy(map);
			return NULL;
		}

		skip_white(self);

		if(!match(self, ':'))
		{
			mutable_string_free(key);
			objectDestroy(map);
			return NULL;
		}
		
		skip_white(self);
		
		value = json_parse_value(self);
		
		if(!value)
		{
			mutable_string_free(key);
			objectDestroy(map);
			return NULL;
		}

		mapInsert(map, key->buffer, value);
		mutable_string_free(key);
		objectDestroy(value);
		skip_white(self);
	}

	skip_white(self);
	return map;
}

static Object*
parse_array(json_parser_t* self)
{
	skip_white(self);
	Object* array = newArray(2);
	Object* value = NULL;

	if(!array)
		return NULL;

	if(self->look == ']')
		return array;

	value = json_parse_value(self);

	if(!value)
	{
		objectDestroy(array);
		return NULL;
	}

	arrayPush(array, value);
	objectDestroy(value);

	while(self->look == ',')
	{
		if(!match(self, ',')) {
			objectDestroy(array);
			return NULL;
		}

		skip_white(self);

		value = json_parse_value(self);

		if(!value) 
		{
			objectDestroy(array);
			return NULL;
		}

		arrayPush(array, value);
		objectDestroy(value);
		skip_white(self);
	}

	skip_white(self);
	return array;
}

static Object*
parse_true(json_parser_t* self)
{
	const char* t = "true";

	while(*t != '\0')
	{
		if(self->look != *t)
			return NULL;

		t++;
		
		parser_next(self);
	}

	return newBool(1);
}

static Object*
parse_false(json_parser_t* self)
{
	const char* t = "false";

	while(*t != '\0')
	{
		if(self->look != *t)
			return NULL;

		t++;
		
		parser_next(self);
	}

	return newBool(0);
}

static Object*
parse_null(json_parser_t* self)
{
	const char* t = "null";

	while(*t != '\0')
	{
		if(self->look != *t)
			return NULL;

		t++;
		
		parser_next(self);
	}

	return newNull();
}

JSON_PARSER_PRIVATE Object* 
json_parse_value(json_parser_t* self)
{
	Object* retval = NULL;
	MutableString* string = NULL;
	double dval = 0;
	bool success = false;

	skip_white(self);

	/* Since we call this function recursed, gotta check if this is first
	 * although the caller could check, but is it appropriate?
	 */
	if(!first_pass) 
	{
		if(!json_value_first_check(self)) 
		{
			return NULL;
		}
	}

	skip_white(self);
	
	if(is_start_of_int(self)) 
	{
		skip_white(self);

		dval = parse_number(self, &success);
		if(!success)
			retval = NULL;
		else
			retval = newDouble(dval);

		skip_white(self);
	}
	else if(is_start_of_string(self))
	{	
		match(self, '"');

		string = parse_string(self);
		if(!string)
		{
			retval = NULL;	
		}
		else
		{		
			retval = newString(string->buffer);
			mutable_string_free(string);		
		}

		if(!match(self, '"'))
		{
			set_error(self, "expecting '\"'");
			objectDestroy(retval);
			retval = NULL;
		}
	}
	else if(is_start_of_object(self))
	{
		match(self, '{');

		skip_white(self);
		
		retval = parse_object(self);

		if(retval == NULL)
		{
			return NULL;
		}

		skip_white(self);

		if(!match(self, '}'))
		{
			set_error(self, "expecting '}'");
			objectDestroy(retval);
			retval = NULL;
		}

		skip_white(self);
	}
	else if(is_start_of_array(self))
	{
		match(self, '[');

		skip_white(self);

		retval = parse_array(self);

		skip_white(self);

		if(!match(self, ']'))
		{
			set_error(self, "expecting ']'");
			objectDestroy(retval);
			retval = NULL;
		}
	
		skip_white(self);
	}
	else if(is_null_or_bool(self))
	{
		switch(self->look)
		{
			case 't':
				retval = parse_true(self);
				if(!retval)
				{
					set_error(self, "unexpected token 't'");
				}
			break;
			case 'f':
				retval = parse_false(self);
				if(!retval)
				{
					set_error(self, "unexpected token 'f'");
				}
			break;
			case 'n':
				retval = parse_null(self);
				if(!retval)
				{
					set_error(self, "unexpected token 'n'");
				}
			break;
		}
	}
	else
	{
		set_error(self, "unexpected token ILLEGAL");
		retval = NULL;
	}

	//skip_white(self);	
	return retval;
}
