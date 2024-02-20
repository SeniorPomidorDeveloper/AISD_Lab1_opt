#ifndef GETSTR_H
#define GETSTR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct string
{
    size_t len;
    char *str;
} string;

bool GetStr(string *str, const char *message);

#endif