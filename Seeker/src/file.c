#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/file.h"
#include "../include/common.h"

#define DEFAULT_NUMBER_LINES 1024
#define DEFAULT_LINE_SIZE 1024

int file_content(const char *file_path, struct wordlist **out_wordlist)
{
    if (file_path == NULL)
    {
        printf("Null file path\n");
        return STATUS_ERROR;
    }

    FILE *fd = fopen(file_path, "r");

    if (fd == NULL)
    {
        perror("Error opening file");
        return STATUS_ERROR;
    }

    struct wordlist *out = (struct wordlist *)malloc(sizeof(struct wordlist));

    char **file_content = malloc(sizeof(char *) * DEFAULT_NUMBER_LINES);

    char c = 0;
    size_t characters_read = 0;
    size_t lines_read = 0;
    size_t iteration_number = 0;

    while (!feof(fd))
    {
        c = fgetc(fd);

        if (characters_read == 0)
        {
            file_content[lines_read] = malloc(DEFAULT_LINE_SIZE);
        }

        // If its not a newline character than we have not reach the end of the line
        if (c != '\n')
        {
            file_content[lines_read][characters_read] = c;

            characters_read++;

            // Detect if more memory needs to be allocated
            if (characters_read % DEFAULT_LINE_SIZE == 0)
            {
                file_content[lines_read] = realloc(file_content[lines_read], characters_read + DEFAULT_LINE_SIZE);
            }
        }
        else
        {
            file_content[lines_read] = realloc(file_content[lines_read], characters_read + 1);

            characters_read = 0;
            lines_read++;

            if (lines_read % DEFAULT_NUMBER_LINES == 0)
            {
                file_content = realloc(file_content, ((sizeof(char *)) * lines_read));
            }
        }

        if (feof(fd))
        {
            file_content = realloc(file_content, sizeof(char *) * lines_read);
        }
    }

    out->content = file_content;
    out->number_of_lines = lines_read;

    *out_wordlist = out;

    fclose(fd);

    return STATUS_SUCCESS;
}