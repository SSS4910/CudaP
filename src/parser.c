#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

#include "sss4910.h"
#include "parser.h"
#include "debug.h"

regex_t regex;
const char *expression = "[^ ]*";

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

/*
 * FUNCTION: parse_init
 * --------------------
 * initializes memory for regular expression structures
 *
 * return:
 *  0 on success
 *  -1 on failure
 */
int
parse_init(){
    if (regcomp(&regex, expression, 0) != 0)
    {
        return -1;
    }
    return 0;
}

/*
 * FUNCTION: parse_teardown
 * ------------------------
 * frees memory allocated for regular expression
 */
void parse_teardown(){
    regfree(&regex);
}
