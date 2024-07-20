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
    printf("\t -d <destination address>\n");
}

static int set_destination(char *destination_address, struct arguments *arguments)
{
    struct addrinfo *res = NULL;
    if (getaddrinfo(destination_address, "443", 0, &res) != 0)
    {
        perror("Error getting address");
        return STATUS_ERROR;
    }

    struct addrinfo *i;

    for (i = res; i != NULL; i = i->ai_next)
    {
        char str[INET6_ADDRSTRLEN];
        if (i->ai_addr->sa_family == AF_INET)
        {
            arguments->destination.ipv4_address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
            memcpy(arguments->destination.ipv4_address, i->ai_addr, sizeof(struct sockaddr_in));

            freeaddrinfo(i);
            return STATUS_SUCCESS;
        }
        else if (i->ai_addr->sa_family == AF_INET6)
        {
            arguments->destination.ipv6_address = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6));
            memcpy(arguments->destination.ipv6_address, i->ai_addr, sizeof(struct sockaddr_in6));

            freeaddrinfo(i);
            return STATUS_SUCCESS;
        }
    }

    freeaddrinfo(i);

    return STATUS_ERROR;
}

int parse_arguments(const int argc, char *argv[], struct arguments **arguments)
{
    struct arguments *out_arguments = (struct arguments *)malloc(sizeof(struct arguments));

    int opt = 0;

    char *host = NULL;

    while ((opt = getopt(argc, argv, "hw:d:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_usage(argv);
            return STATUS_ERROR;
        case 'd':
            host = optarg;
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

    if (host == NULL)
    {
        printf("Missing destination address!\n");
        print_usage(argv);

        return STATUS_ERROR;
    }

    if (out_arguments->wordlist_path == NULL)
    {
        printf("Missing wordlist path!\n");
        print_usage(argv);

        return STATUS_ERROR;
    }

    if (set_destination(host, out_arguments) != STATUS_SUCCESS)
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
    
    if (arguments->destination.ipv6_address != NULL)
    {
        free(arguments->destination.ipv6_address);
    }

    free(arguments);

    return STATUS_SUCCESS;
}