#include <object.h>
#include <jsonparser.h>
#include <easyio.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_common.h"

struct file_name
{
	const char* name;
	size_t length;
};

static struct file_name files[] = {
	{"file/pass3.json", sizeof("file/pass3.json") -1},
	{"dns.json", 0},
	{NULL, 0}
};


int main(void)
{

	int i;
	char* contents;
	for(i = 0; files[i].name != NULL; i++)
	{
		Object* json = NULL;
		json_error_t error;

		const char* path;
		
		path = files[i].name;

		printf("path=%s\n", path);

		contents = file_get_contents(path);
		
		if(!contents)
		{	
			printf("file_get_contents: can't open file %s\n", path);
			return 1;
		}
		json = json_parse(contents);

		if(json == NULL)
		{
			json_last_error(&error);

			printf("json_parse: %s on line %zu, column %zu, token '%c', in file %s\n", 
					error.message, error.line, error.column, error.token, path);

			printf("*********** CONTENTS *********** \n");
			printf("%s", contents);
			printf("\n********* END CONTENTS ******* \n");
			free(contents);
			return 1;
		}

		free(contents);

		size_t length;
		char* text = objectToJson(json, 1, &length);

		objectDestroy(json);

		if(!text)
		{
			printf("objectToJson: retuned NULL\n");
			return 1;
		}

		printf("%s\n", text);
		free(text);
	}
	
	return 0;
}
