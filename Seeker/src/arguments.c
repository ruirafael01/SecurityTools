#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "../include/arguments.h"

static void print_usage(char *argv[])
{
    printf("Usage of %s:\n", argv[0]);
    printf("\t -h for Help\n");
    printf("\t -w <PATH to wordlist>\n");
    printf("\t -u <URL>\n");
}

int parse_arguments(const int argc, char *argv[], struct arguments **arguments)
{
    struct arguments *out_arguments = (struct arguments *)malloc(sizeof(struct arguments));

    int opt = 0;

    while ((opt = getopt(argc, argv, "hw:u:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_usage(argv);
            return STATUS_ERROR;
        case 'u':
            out_arguments->url = optarg;
            break;
        case 'w':
            out_arguments->wordlist_path = optarg;
            break;
        case '?':
            printf("Unknown option: %c\n", optopt);
            print_usage(argv);
            return STATUS_ERROR;
            break;
        default:
            print_usage(argv);
            return STATUS_ERROR;
            break;
        }
    }

    if (out_arguments->url == NULL)
    {
        printf("Missing URL!\n");
        print_usage(argv);

        return STATUS_ERROR;
    }

    if (out_arguments->wordlist_path == NULL)
    {
        printf("Missing wordlist path!\n");
        print_usage(argv);

        return STATUS_ERROR;
    }

    *arguments = out_arguments;

    return STATUS_SUCCESS;
};

int delete_arguments(struct arguments *arguments)
{
    if (arguments == NULL)
    {
        return STATUS_ERROR;
    }

    free(arguments);

    return STATUS_SUCCESS;
}