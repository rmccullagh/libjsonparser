/**
 * @file jsonparser.h
 * @author Ryan McCullagh
 * @brief Public API function of the jsonparser library
 * 
 * @section LICENSE
 *
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
 *
 * @section DESCRIPTION
 *
 * The json-parser (jsonparser) library parses JSON text into a useful i
 * Object structure (http://github.com/libobject/libobject) provided that 
 * the JSON is correct as dictated by the specification located at 
 * https://www.ietf.org/rfc/rfc4627.txt
 */

#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>

#include <object.h>

typedef struct json_error_t {
	char* message;
	size_t	    column;
	size_t		  line;
	char				token;
} json_error_t;

#ifdef HAVE_JSON_PARSER_DEBUG
  #define JSON_PARSER_DEBUG 1
#else
  #define JSON_PARSER_DEBUG 0
#endif

#if defined _WIN32 || defined __CYGWIN__
  #define JSON_PARSER_HELPER_DLL_IMPORT __declspec(dllimport)
  #define JSON_PARSER_HELPER_DLL_EXPORT __declspec(dllexport)
  #define JSON_PARSER_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define JSON_PARSER_HELPER_DLL_IMPORT \
			__attribute__ ((visibility ("default")))
    #define JSON_PARSER_HELPER_DLL_EXPORT \
			__attribute__ ((visibility ("default")))
    #define JSON_PARSER_HELPER_DLL_LOCAL \
			__attribute__ ((visibility ("hidden")))
  #else
    #define JSON_PARSER_HELPER_DLL_IMPORT
    #define JSON_PARSER_HELPER_DLL_EXPORT
    #define JSON_PARSER_HELPER_DLL_LOCAL
  #endif
#endif

#ifdef BUILDING_JSON_PARSER
  #define JSON_PARSER_API JSON_PARSER_HELPER_DLL_EXPORT
#else
  #define JSON_PARSER_API JSON_PARSER_HELPER_DLL_IMPORT
#endif

#define JSON_PARSER_PRIVATE JSON_PARSER_HELPER_DLL_LOCAL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Parse JSON text into an Object.
 *
 * This function will try to parse a JSON text buffer. It will return an 
 * Object if the parsing was succesfull, otherwise it will return NULL 
 * and a json_error structure will be populated and available to be gotten 
 * by calling json_last_error 
 * 
 * @param source The JSON text terminated by a NULL byte
 * @return NULL|Object
 */
JSON_PARSER_API 
Object* json_parse(const char* source);

/**
 * @brief Set a json_error_t structure populated with useful information
 
 * This function will take a json_error_t pointer and populate it's fields 
 * with the error information returned by the internal parser
 *
 * @param error A pointer to a json_error_t structure
 * @return void
 */
JSON_PARSER_API
void json_last_error(json_error_t* error);

#ifdef __cplusplus
}
#endif

#endif /* __JSON_PARSER_H__ */
