#include <stdio.h>

#include "sss4910.h"
#include "debug.h"
#include "requesttofile.h"

FILE *parserOutput;

//opens a file for parser output
int 
open_parser_output(char *filename){
    if ((parserOutput = fopen(filename, "w")) == NULL)
    {
        errlog_write("failed to open parser output file\n");
        return -1;
    }
    return 0;
}

//closes the parser output file
int
close_parser_output(){
    if (fclose(parserOutput) != 0)
    {
        errlog_write("failed to close parser output file\n");
        return -1;
    }
    return 0;
}

//prints the parser output file header
int
print_header(){
    const char *header = "Contents of Buffer";
    fputs(header, parserOutput);
    return 0;
}

//prints a request to file
int
print_request(Request *request){
    fprintf(parserOutput, "%s", request->host);
    fprintf(parserOutput, "%s", request->country);
    fprintf(parserOutput, "%s", request->region);
    return 0;
}
