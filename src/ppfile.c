#include <stdio.h>

#include "sss4910.h"
#include "debug.h"
#include "ppfile.h"

FILE *parserOutput;

/*
 * FUNCTION: pp_open
 * -----------------
 * creates and opens a pretty print output file
 *
 * filename: filename string for file creation
 *
 * returns: 0 on successful completion, -1 on failure
 */
int 
open_parser_output(char *filename){
    if ((parserOutput = fopen(filename, "w")) == NULL)
    {
        debug_write("failed to open parser output file\n");
        return -1;
    }
    return 0;
}


/*
 * FUNCTION: pp_close
 * ------------------
 * closes the pretty print output file
 *
 * returns: 0 on successful completion, -1 on failure
 */
int
pp_close(){
    if (fclose(parserOutput) != 0)
    {
        debug_write("failed to close parser output file\n");
        return -1;
    }
    return 0;
}

/*
 * FUNCTION: pp_header
 * ----------------------
 * pretty prints the table header
 *
 * returns: 0 on successful completion, -1 on failure
 */
int
pp_header(){
    const char *header = "Parsed Requests\n";
    fputs(header, parserOutput);
    pp_line();
    fputs("|", parserOutput);
    fprintf(parserOutput, "%15.15s|", "HOST");
    fprintf(parserOutput, "%32.32s|", "CLIENT ID");
    fprintf(parserOutput, "%32.32s|", "USER ID");
    fprintf(parserOutput, "%32.32s|", "REQUEST");
    fprintf(parserOutput, "%3.3s|", "RET");
    fprintf(parserOutput, "%4.4s|\n", "SIZE");
    return 0;
}

/*
 * FUNCTION: pp_line
 * --------------------
 * prints a line of hyphens
 *
 * returns: 0 on successful completion, -1 on failure
 */
int
pp_line()
{
    int i;
    for (i = 0; i < 125; i++)
    {
        fprintf(parserOutput, "%s", "-");
    }
    fputs("\n", parserOutput);
    return 0;
}

/*
 * FUNCTION: pp_request
 * -----------------------
 * pretty prints a request structure to file
 *
 * request: a pointer to a request structure
 *
 * returns: 0 on successful completion, -1 on failure
 */
int
pp_request(Request *request){
    pp_line();

    fputs("|", parserOutput);
    fprintf(parserOutput, "%15.15s|", request->host);
    fprintf(parserOutput, "%32.32s|", request->clientId);
    fprintf(parserOutput, "%32.32s|", request->userId);
    fprintf(parserOutput, "%32.32s|", request->req);
    fprintf(parserOutput, "%3d|", request->retCode);
    fprintf(parserOutput, "%4d|\n", request->dataSize);

    return 0;
}
