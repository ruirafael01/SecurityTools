#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <netdb.h>
#include <stdbool.h>

#include "common.h"

struct arguments
{
    char *url;
    char *wordlist_path;
    bool http_codes[500];
};

int parse_arguments(const int argc, char *argv[], struct arguments **arguments);

int delete_arguments(struct arguments *arguments);

#endif