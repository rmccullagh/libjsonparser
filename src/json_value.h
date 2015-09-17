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
#ifndef __JSON_VALUE_H__
#define __JSON_VALUE_H__

#include "json_internal.h"

JSON_PARSER_PRIVATE
Object* json_parse_value(json_parser_t*);

JSON_PARSER_PRIVATE
void json_value_reset(void);

#endif /* __JSON_VALUE_H__ */
