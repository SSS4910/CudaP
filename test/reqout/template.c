#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../src/sss4910.h"
#include "../../src/debug.h"
#include "../../src/requesttofile.h"
#include "../../src/parser.h"

int
main(int argc, char **argv){
    char *filename = "output.txt";
    char *host = "127.0.0.1";
    char *country = "United States";
    char *region = "Michigan";
    char *request = "GET stuff";
    int retCode = 200;
    int dataSize = 2048;
    char * referer = "http://www.google.com";
    char * userAgent = "Mozilla 5.0";
    Request req;

    req.host = host;
    req.country = country;
    req.region = region;
    req.req = request;
    req.httpReturnCode = retCode;
    req.dataSize = dataSize;
    req.referer = referer;
    req.userAgent = userAgent;

    open_parser_output(filename);
    print_header();

    print_request(&req);
    close_parser_output();
    return 0;
}
