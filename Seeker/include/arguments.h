#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <netdb.h>

#include "common.h"

union destination_address
{
    struct sockaddr_in *ipv4_address;
    struct sockaddr_in6 *ipv6_address;
};

struct arguments
{
    union destination_address destination;
    char *wordlist_path;
};

int parse_arguments(const int argc, char *argv[], struct arguments **arguments);

int delete_arguments(struct arguments *arguments);

#endif