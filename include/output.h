#ifndef OUTPUT_H
#define OUTPUT_H

#include "dataset.h"

void print_separator(int name_width);
void print_header(int name_width);
void print_row(int name_width, char *name, double mean, double stddev, double min, double max, double median, double q1, double q2,  double q3);
void print_descriptives(Dataset *dataset, int *columns, int n);
void print_frequencies(Dataset *dataset, int col);
void print_independent_ttest(Dataset *dataset, int col_x, int col_y, double t_value, double p_value);
void print_chisquare(Dataset *dataset, int col_x, int col_y, double chi_sq, double p_value);
void print_anova(Dataset *dataset, int col_x, int col_y, double F_value, double p_value);
void print_paired_ttest(Dataset *dataset, int col_x, int col_y, double t_value, double p_value);

#endif