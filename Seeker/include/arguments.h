#ifndef ARGUMENTS_H
#define ARGUMENTS_H
#include <netdb.h>

#include "common.h"

struct arguments
{
    char *url;
    char *wordlist_path;
};

int parse_arguments(const int argc, char *argv[], struct arguments **arguments);

int delete_arguments(struct arguments *arguments);

#endif