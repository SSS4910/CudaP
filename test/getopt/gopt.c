#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
print_usage(char *name){
    printf("usage: %s [-f file] [-h]\n", name);
}

void
print_help(){
    printf("parses and analyzes CLF Apache Log File\n");
    printf("Written by Matthew Sarver, Michael Shields, and John Stanley\n");
    printf("For CS4900 & CS4910\n");
    printf("See README for more.\n");
}

int
main(int argc, char **argv)
{
    int c;             /* option character */
    char *fflag;

    while ((c = getopt(argc, argv, ":hf:")) != -1)
    {
        switch (c)
        {
            case 'f':
                /* handle -a, set a flag, whatever */
                fflag = optarg;
                printf("%s\n", fflag);
                break;
            case 'h':
                print_usage(argv[0]);
                print_help();
                return 1;
            case ':':
                /* missing option argument */
                print_usage(argv[0]);
                return 1;
            case '?':
            default:
                /* invalid option */
                print_usage(argv[0]);
                return 1;
        }
    }
    printf("extra\n");
    return 0;
}
