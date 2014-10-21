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
    const char *header = "Parsed Requests\n";
    fputs(header, parserOutput);
    print_ascii_row();
    fputs("|", parserOutput);
    fprintf(parserOutput, "%15.15s|", "HOST");
    fprintf(parserOutput, "%32.32s|", "COUNTRY");
    fprintf(parserOutput, "%32.32s|", "REGION");
    fprintf(parserOutput, "%32.32s|", "REQUEST");
    fprintf(parserOutput, "%3.3s|", "RET");
    fprintf(parserOutput, "%4.4s|\n", "SIZE");
    return 0;
}

int
print_ascii_row()
{
    int i;
    for (i = 0; i < 125; i++)
    {
        fprintf(parserOutput, "%s", "-");
    }
    fputs("\n", parserOutput);
    return 0;
}

//prints a request to file
int
print_request(Request *request){
    print_ascii_row();

    fputs("|", parserOutput);
    fprintf(parserOutput, "%15.15s|", request->host);
    fprintf(parserOutput, "%32.32s|", request->country);
    fprintf(parserOutput, "%32.32s|", request->region);
    fprintf(parserOutput, "%32.32s|", request->req);
    fprintf(parserOutput, "%3d|", request->httpReturnCode);
    fprintf(parserOutput, "%4d|\n", request->dataSize);

    return 0;
}
