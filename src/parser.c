#include <stdio.h>
#include <stdlib.h>

#include "sss4910.h"
#include "parser.h"
#include "debug.h"

/*
 * FUNCTION: parse
 * --------------
 * parses a NCSA Common Log Format string into a Request structure
 *
 * line: a string containing a NCSA Common Log Format access line
 *
 * returns: a pointer to a Request structure
 */
Request *
parse(char * line){
    //int i = 0;
    //int field = 0;
    //int whiteSpaceIndex = 0;
    Request * request = malloc(sizeof (Request *));
    //for char c in line:
    //  if c is ":
    //      ignore whitespace == true
    //  elif ignorewhitespace == true && c is ":
    //      ignore whitespace == false
    //  elif ignorewhitespace == false && c is whitespace:
    //      mark whitespace index
    //      strip out substring
    //  else:
    //      continue
    return request;
}
