# libjsonparser
JSON parser in C implements https://www.ietf.org/rfc/rfc4627.txt. It parses JSON text into a C representation. The data structure is a C struct named `Object`, which is a C library located at [https://github.com/libobject](https://github.com/libobject/libobject). `Object` supports every value that JSON has and even more.

# Dependencies
- libobject - https://github.com/libobject
- libmutablestring - https://github.com/rmccullagh/libmutablestring
- autotools
- libtool

# Install
- git clone https://github.com/rmccullagh/jsonparser
- ./autogen.sh
- make
- make check
- sudo make install

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
