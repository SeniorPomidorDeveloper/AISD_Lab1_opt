#include "int_matrix_from_bin_file.h"

#include <string.h>

bool MatrixInit(intMatrix *matrix, FILE *file)
{
    if (fread(&(matrix->len), sizeof(size_t), 1, file) == 0) {
        return false;
    }
    if (matrix->len != 0) {
        matrix->lines = (intLine *) malloc(matrix->len * sizeof(intLine));
        if (!matrix->lines) {
            return false;
        }
        for (size_t i = 0; i < matrix->len; ++i) {
            if (fread(&(matrix->lines[i].offset), sizeof(long int), 1, file) == 0) {
                return false;
            }
            if (fread(&(matrix->lines[i].len), sizeof(size_t), 1, file) == 0) {
                return false;
            }
        }
    }
    return true;
}

bool FindMinInIntLine(const intLine *inputLine, intLine *outputLine, FILE *file)
{
    long int offsetMin = inputLine->offset;
    long long int minValue;
    if (fseek(file, offsetMin, SEEK_SET) != 0) {
        return false;
    }
    if (fread(&minValue, sizeof(long long int), 1, file) == 0) {
        return false;
    } 
    for (long int i = inputLine->offset + sizeof(long long int); 
         i < inputLine->offset + (long int) (inputLine->len * sizeof(long long int)); i += sizeof(long long int)) {
        long long int num;
        if (fseek(file, i, SEEK_SET) != 0) {
            return false;
        }
        if (fread(&num, sizeof(long long int), 1, file) == 0) {
            return false;
        }
        if (minValue > num) {
            minValue = num;
            offsetMin = i;
        }
    }
    outputLine->len = inputLine->len - (size_t) ((offsetMin - inputLine->offset) / sizeof(long int));
    outputLine->offset = offsetMin;
    return true;
}