#include <stdio.h>
#include <stdlib.h>
#include "dataset.h"

void dataset_free(Dataset *dataset) {
    for (int i = 0; i < dataset->num_cases; i++) {
        free(dataset->cases[i].values);
    }
    free(dataset->cases);
    free(dataset->vars);
}