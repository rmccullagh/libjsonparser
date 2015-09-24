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

#include <easyio.h>
#include <jsonparser.h>
#include <stdlib.h>
#include <stdio.h>
#include <object.h>

int main(int argc, char** argv)
{

	if(argc < 2) {
		printf("usage: %s file ...\n", argv[0]);
		return 0;
	}	

	argc--;
	argv++;

	int i;

	char* contents;
	Object* o;
	size_t length;
	char* c;
	json_error_t error;

	for(i = 0; i < argc; i++)
	{
		contents = file_get_contents(argv[i]);

		if(!contents) {
			printf("file_get_contents: file '%s' not found\n", argv[i]);
			continue;

		}

		o = json_parse(contents);

		if(!o)
		{
			json_last_error(&error);
			printf("json_parse: %s on line %zu, column=%zu, token=%c\n",
					error.message, error.line, error.column, error.token);
			free(error.message);

			free(contents);
			continue;

		}
		
		c = objectToJson(o, 1, &length);

		printf("%s\n", c);

		free(c);

		free(contents);

		objectDestroy(o);

		
	}

	return 0;
}
