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
    printf("\t -c <List of HTTP codes to match against>");
}

static int set_http_codes(char *http_codes, struct arguments *arguments)
{
    memset(arguments->http_codes, false, sizeof(500));

    if (http_codes == NULL)
    {
        // SET HTTP codes from 200 to 399 as true
        memset(arguments->http_codes, true, 399);

        return STATUS_SUCCESS;
    }

    char *current_string = strtok(http_codes, ",");

    int number = 0;

    while (current_string != NULL)
    {
        number = atoi(current_string);

        if (number == 0)
        {
            printf("Error parsing number from string %s\n", current_string);
            return STATUS_ERROR;
        }

        arguments->http_codes[number] = true;

        current_string = strtok(NULL, ",");
    }
    
    return STATUS_SUCCESS;
}

int parse_arguments(const int argc, char *argv[], struct arguments **arguments)
{
    struct arguments *out_arguments = (struct arguments *)malloc(sizeof(struct arguments));

    int opt = 0;

    char *http_codes = NULL;

    while ((opt = getopt(argc, argv, "hw:u:c:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_usage(argv);
            return STATUS_ERROR;
        case 'u':
            out_arguments->url = optarg;
            break;
        case 'c':
            http_codes = optarg;
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

    if (set_http_codes(http_codes, out_arguments) != STATUS_SUCCESS)
    {
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