# jsonparser
JSON parser in C

# Example

```c
#include <object.h>
#include <jsonparser.h>
#include <stdio.h>

static const char* source = "{\"name\": \"Ryan\"}";

int main(void)
{
	Object* JSON = json_parse(source);
	json_error_t error;

	if(!JSON)
	{
		json_last_error(&error);
		printf("json_parse: %s on line %zu, column %zu, token '%c'\n", 
				error.message, error.line, error.column, error.token);
		return 1;
	}

	OBJECT_DUMP(JSON);
	
	objectDestroy(JSON);
	
	return 0;
}
```
