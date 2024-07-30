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