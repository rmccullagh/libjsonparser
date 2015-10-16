Welcome to the `example` directory. Here you will find a program that will
take an arbitary amount of command line arguments that are strings to
file names. From there we'll call the `json_parse` function on the contents
of that file for each. 

## Requirements
- libobject - http://github.com/libobject/libobject
- libeasyio - http://github.com/rmccullagh/easyio
- libjsonparser - http://github.com/rmccullagh/jsonparser


What happens is the `json_parse` function will parse the file into an `Object`, 
see the `libobject` library for a complete description. From there you can
use the `libobject` API to access the values of the JSON object.

## Building
- `./build`
