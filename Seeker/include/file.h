#ifndef FILE_H
#define FILE_H

struct wordlist{ 
    size_t number_of_lines;
    char **content;
};

int file_content(const char *file_path, struct wordlist **out_wordlist);

#endif