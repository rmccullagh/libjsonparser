noinst_HEADERS = json_internal.h
lib_LTLIBRARIES = libjsonparser.la
libjsonparser_la_SOURCES = json_internal.h jsonparser.c
libjsonparser_la_CFLAGS = $(AM_CFLAGS)
libjsonparser_la_CPPFLAGS = -DBUILDING_JSON_PARSER
libjsonparser_la_LDFLAGS = -no-undefined -version-info ${JSON_PARSER_VERSION}
include_HEADERS = jsonparser.h
