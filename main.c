#include <stdio.h>

#include "libs/int_matrix_from_bin_file.h"
#include "libs/getStr.h"

void PrintMatrix(const intMatrix *matrix);

bool Process(const intMatrix *matrix, FILE *inputFile, FILE *outputFile);

enum PROGRAM_STATUS
{
ERROR = 1,
SUCCESSFULLY = 0
};

int main()
{
    string pathInputFile = {0, NULL};
    if (!GetStr(&pathInputFile, "Введите путь до файла ввода: ")) {
        return ERROR;
    }
    FILE *inputFile = fopen(pathInputFile.str, "rb");
    if (!inputFile) {
        printf("[ERROR] Такого файла не существует или произошла ошибка во время открытия файла!\n");
        free(pathInputFile.str);
        return ERROR;
    }
    intMatrix matrix = {0, NULL};
    if (!MatrixInit(&matrix, inputFile)) {
        printf("[ERROR] Произошла ошибка во время инициализации матрицы!\n");
        free(pathInputFile.str);
        fclose(inputFile);
        return ERROR;
    }
    string pathOutputFile = {0, NULL};
    if (!GetStr(&pathOutputFile, "Введите путь до файла вывода: ")) {
        free(matrix.lines);
        free(pathInputFile.str);
        fclose(inputFile);
        return ERROR;
    }
    FILE *outputFile = fopen(pathOutputFile.str, "wb");
    if (!outputFile) {
        printf("[ERROR] Такого файла не существует или произошла ошибка во время открытия файла!\n");
        free(matrix.lines);
        free(pathInputFile.str);
        free(pathOutputFile.str);
        fclose(inputFile);
        return ERROR;
    }
    if (!Process(&matrix, inputFile, outputFile)) {
        printf("[ERROR] Произошла ошибка во время обработки матрицы!\n");
        free(matrix.lines);
        free(pathInputFile.str);
        free(pathOutputFile.str);
        fclose(inputFile);
        fclose(outputFile);
        return ERROR;
    }
    free(matrix.lines);
    free(pathInputFile.str);
    free(pathOutputFile.str);
    fclose(inputFile);
    fclose(outputFile);
    return SUCCESSFULLY;
}

bool Process(const intMatrix *matrix, FILE *inputFile, FILE *outputFile)
{
    size_t len = 2;
    if (fwrite(&len, sizeof(size_t), 1, outputFile) != 1) {
        return false;
    }
    intMatrix newMatrix = {matrix->len, NULL};
    newMatrix.lines = (intLine *) malloc(matrix->len * sizeof(intLine));
    for (size_t i = 0; i < matrix->len; ++i) {
        newMatrix.lines[i].len = 0;
        newMatrix.lines[i].offset = 0;
        if (!FindMinInIntLine(&(matrix->lines[i]), &(newMatrix.lines[i]), inputFile)) {
            return false;
        }
        if (fwrite(&(newMatrix.lines[i].offset), sizeof(long int), 1, outputFile) == 0) {
            return false;
        }
        if (fwrite(&(newMatrix.lines[i].len), sizeof(size_t), 1, outputFile) == 0) {
            return false;
        }
    } 
    long long int *array = NULL;
    for (size_t i = 0; i < matrix->len; ++i) {
        array = (long long int *) realloc(array, newMatrix.lines[i].len * sizeof(long long int));
        if (!array) {
            return false;
        }
        if (fseek(inputFile, newMatrix.lines[i].offset, SEEK_SET) != 0) {
            return false;
        }
        for (size_t j = 0; j < newMatrix.lines[i].len; ++j) {
            if (fread(array + j, sizeof(long long int), 1, inputFile) == 0) {
                return false;
            }
        }
        for (size_t j = 0; j < newMatrix.lines[i].len; ++j) {
            if (fwrite(array + j, sizeof(long long int), 1, outputFile) == 0) {
                return false;
            }
        }
    }
    free(array);
    free(newMatrix.lines);
    return true;
}