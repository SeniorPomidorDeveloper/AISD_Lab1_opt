#ifndef INT_MATRIX_FROM_BIN_FILE_H
#define INT_MATRIX_FROM_BIN_FILE_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct intLine
{
    size_t len;
    long int offset;
} intLine;

typedef struct intMatrix
{
    size_t len;
    intLine *lines;
} intMatrix;

bool MatrixInit(intMatrix *matrix, FILE *file);
bool FindMinInIntLine(const intLine *inputLine, intLine *outputLine, FILE *file);

#endif