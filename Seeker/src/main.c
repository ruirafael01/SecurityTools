#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "../include/arguments.h"
#include "../include/file.h"
#include "../include/seek.h"

static void cleanup(struct arguments *arguments, struct wordlist *wordlist)
{
    if (arguments != NULL)
    {
        delete_arguments(arguments);
    }

    if (wordlist != NULL)
    {
        delete_worldist(wordlist);
    }
}

int main(int argc, char *argv[])
{
    struct arguments *arguments = NULL;

    if (parse_arguments(argc, argv, &arguments) != STATUS_SUCCESS)
    {
        exit(EXIT_FAILURE);
    }

    printf("Path to wordlist %s\n", arguments->wordlist_path);

    if (arguments->destination.ipv4_address != NULL)
    {
        char address[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &arguments->destination.ipv4_address->sin_addr, address, sizeof(address));

        printf("IPv4 address: %s\n", address);
    }
    else
    {
        char address[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &arguments->destination.ipv6_address->sin6_addr, address, sizeof(address));

        printf("IPv6 address: %s\n", address);
    }

    struct wordlist *wordlist = NULL;

    if (file_content(arguments->wordlist_path, &wordlist) != STATUS_SUCCESS)
    {
        exit(EXIT_FAILURE);
        cleanup(arguments, wordlist);
    }

    if (seek(arguments, wordlist) != STATUS_SUCCESS)
    {
        cleanup(arguments, wordlist);
        exit(EXIT_FAILURE);
    }

    cleanup(arguments, wordlist);
}