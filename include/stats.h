#ifndef STATS_H
#define STATS_H

#include "dataset.h"

typedef struct {
    double value;
    int count;
} FreqEntry;

double mean(Dataset *dataset, int idx);
double stddev(Dataset *dataset, int idx);
double min_val(Dataset *dataset, int idx);
double max_val(Dataset *dataset, int idx);
double median(Dataset *dataset, int idx);
void quartiles(Dataset *dataset, int idx, double *q1, double *q2, double *q3);
FreqEntry *frequencies(Dataset *dataset, int idx, int *out_size);
double correlation(Dataset *dataset, int idx_x, int idx_y);
void independent_t_test(Dataset *dataset, int idx_x, int idx_y, double *t_value, double *p_value);
void chi_square(Dataset *dataset, int idx_x, int idx_y, double *chi_sq, double *p_value);
void anova(Dataset *dataset, int idx_x, int idx_y, double *F_value, double *p_value);
void paired_t_test(Dataset *dataset, int idx_x, int idx_y, double *t_value, double *p_value);

#endif