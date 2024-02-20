#include "getStr.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool GetStr(string *str, const char *message)
{
    printf("%s", message);
    const size_t LEN_BLOCK = 10;
    char symbol = getchar();
    if (symbol == EOF) {
        return false;
    }
    str->len = 0;
    str->str = NULL;
    if (symbol != '\n') {
        str->str = (char *) malloc(LEN_BLOCK * sizeof(char));
        if (!str->str) {
            return false;
        }
        do {
            str->str[str->len] = symbol;
            str->len++;
            if (str->len % LEN_BLOCK == 0) {
                str->str = (char *) realloc(str->str, (str->len + LEN_BLOCK) * sizeof(char));
                if (!str->str) {
                    return false;
                }
            }
            symbol = getchar();
            if (symbol == EOF) {
                free(str->str);
                return false;
            }
        } while (symbol != '\n');
        str->str = (char *) realloc(str->str, (str->len + 1) * sizeof(char));
        if (!str->str) {
            return false;
        }
        str->str[str->len] = 0;
    }
    return true;
}