#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"

void parse_csv(Dataset *dataset, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return;
    }

    char line[1024];
    int is_header = 1;

    dataset->vars = malloc(64 * sizeof(Variable));
    dataset->cases = malloc(4 * sizeof(Case));
    dataset->capacity = 4;
    dataset->num_cases = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (is_header) {
            // 헤더 파싱
            int i = 0;
            char *token = strtok(line, ",\n");
            while (token != NULL) {
                strcpy(dataset->vars[i].name, token);
                dataset->vars[i].type = 'N'; // N: number, S: string
                i++;
                token = strtok(NULL, ",\n");
            }
            dataset->num_vars = i;
            is_header = 0;
        }
        else {
            // 데이터 파싱
            Case c;
            c.values = malloc(dataset->num_vars * sizeof(double));
            int i = 0;
            char *token = strtok(line, ",\n");
            while (token != NULL) {
                c.values[i] = atof(token);
                i++;
                token = strtok(NULL, ",\n");
            }
            // 공간이 꽉 찼으면 realloc
            if (dataset->num_cases == dataset->capacity) {
                dataset->capacity *= 2;
                dataset->cases = realloc(dataset->cases,
                    dataset->capacity * sizeof(Case));
            }
            dataset->cases[dataset->num_cases++] = c;
        }
    }

    fclose(fp);
}