#include <stdlib.h>
#include <stdio.h>
#include <object.h>
#include <jsonparser.h>
#include <easyio.h>

int main(int argc, char** argv)
{
	if(argc < 2) {
		printf("Usage: ./%s FILE ...\n", argv[0]);
		return 1;
	}

	argc--;
	argv++;

	int i;
	char* contents;
	Object* value;
	json_error_t error;
	
	for(i = 0; i < argc; i++)
	{
		contents = file_get_contents(argv[i]);

		if(!contents) {

			printf("warning: file '%s' not found\n", argv[i]);

			continue;
		}
		
		value = json_parse(contents);

		free(contents);

		if(!value) {
			json_last_error(&error);
			printf("json_parse: %s on line %zu at column=%zu",
				error.message, error.line, error.column);

			free(error.message);

			continue;

		}

		OBJECT_DUMP(value);

		objectDestroy(value);						
	}

	return 0;
}
