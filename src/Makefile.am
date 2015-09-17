noinst_HEADERS = \
	json_value.h 		\
	json_internal.h \
	json_version.h	\
	$(NULL)

lib_LTLIBRARIES = libjsonparser.la
libjsonparser_la_SOURCES = \
	json_value.c 					\
	json_value.h 					\
	json_version.h				\
	json_internal.c				\
	json_internal.h 			\
	jsonparser.c					\
	$(NULL)

libjsonparser_la_CFLAGS = $(AM_CFLAGS)
libjsonparser_la_CPPFLAGS = -DBUILDING_JSON_PARSER
libjsonparser_la_LDFLAGS = -no-undefined -version-info ${JSON_PARSER_VERSION}
include_HEADERS = jsonparser.h
