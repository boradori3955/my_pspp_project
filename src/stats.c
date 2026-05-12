#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_cdf.h>
#include "stats.h"

double mean(Dataset *dataset, int idx) {
    double sum = 0;
    for (int j = 0; j < dataset->num_cases; j++) {
        sum += dataset->cases[j].values[idx];
    }
    return sum / dataset->num_cases;
}

double stddev(Dataset *dataset, int idx) {
    double avg = mean(dataset, idx);
    double sum = 0;
    for (int j = 0; j < dataset->num_cases; j++) {
        double diff = dataset->cases[j].values[idx] - avg;
        sum += (diff * diff);
    }
    return sqrt(sum / dataset->num_cases);
}

double min_val(Dataset *dataset, int idx) {
    double min = dataset->cases[0].values[idx];
    for (int j = 1; j < dataset->num_cases; j++) {
        if (dataset->cases[j].values[idx] < min) {  
            min = dataset->cases[j].values[idx];
        }
    }
    return min;
}

double max_val(Dataset *dataset, int idx) {
    double max = dataset->cases[0].values[idx];
    for (int j = 1; j < dataset->num_cases; j++) {
        if (dataset->cases[j].values[idx] > max) {  
            max = dataset->cases[j].values[idx];
        }
    }
    return max;
}

int compare(const void *a, const void *b) {
    double da = *(double *)a;
    double db = *(double *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

double median(Dataset *dataset, int idx) {
    int n = dataset->num_cases;
    
    double *tmp = malloc(n * sizeof(double));
    for (int j = 0; j < n; j++) {
        tmp[j] = dataset->cases[j].values[idx];
    }
    
    qsort(tmp, n, sizeof(double), compare);
    
    double result;
    if (n % 2 == 0) {
        result = (tmp[n/2] + tmp[n/2 - 1]) / 2;  // 짝수일 때
    } else {
        result = tmp[n/2];  // 홀수일 때
    }
    
    free(tmp);
    return result;
}

void quartiles(Dataset *dataset, int idx, double *q1, double *q2, double *q3) {
    int n = dataset->num_cases;
    
    double *tmp = malloc(n * sizeof(double));
    for (int j = 0; j < n; j++) {
        tmp[j] = dataset->cases[j].values[idx];
    }
    
    qsort(tmp, n, sizeof(double), compare);
    
    *q1 = tmp[n/4];
    *q2 = tmp[n/2];
    *q3 = tmp[3*n/4];
    
    free(tmp);
}

FreqEntry *frequencies(Dataset *dataset, int idx, int *out_size) {
    int n = dataset->num_cases;
    FreqEntry *freq = malloc(n * sizeof(FreqEntry));
    int size = 0;

    for (int j = 0; j < n; j++) {
        double val = dataset->cases[j].values[idx];
        int found = 0;

        for (int k = 0; k < size; k++) {
            if (val == freq[k].value) {
                freq[k].count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            freq[size].value = val;
            freq[size].count = 1;
            size++;
        }
    }

    *out_size = size;
    return freq;
}

double correlation(Dataset *dataset, int idx_x, int idx_y) {
    double mean_x = mean(dataset, idx_x);
    double mean_y = mean(dataset, idx_y);
    double std_x = stddev(dataset, idx_x);
    double std_y = stddev(dataset, idx_y);
    
    double sum = 0;
    for (int j = 0; j < dataset->num_cases; j++) {
        sum += (dataset->cases[j].values[idx_x] - mean_x) * (dataset->cases[j].values[idx_y] - mean_y);
    }
    
    return sum / (dataset->num_cases * std_x * std_y);
}

void independent_t_test(Dataset *dataset, int idx_x, int idx_y, double *t_value, double *p_value) {
    int n = dataset->num_cases;
 
    double group_vals[2];
    int num_groups = 0;
    for (int j = 0; j < n && num_groups < 2; j++) {
        double v = dataset->cases[j].values[idx_x];
        int found = 0;
        for (int k = 0; k < num_groups; k++) {
            if (group_vals[k] == v) { found = 1; break; }
        }
        if (!found) group_vals[num_groups++] = v;
    }
 
    double *g1 = malloc(n * sizeof(double));
    double *g2 = malloc(n * sizeof(double));
    int n1 = 0, n2 = 0;
    for (int j = 0; j < n; j++) {
        double gv = dataset->cases[j].values[idx_x];
        double yv = dataset->cases[j].values[idx_y];
        if (gv == group_vals[0]) g1[n1++] = yv;
        else                     g2[n2++] = yv;
    }
 
    double mean1 = gsl_stats_mean(g1, 1, n1);
    double mean2 = gsl_stats_mean(g2, 1, n2);
    double var1  = gsl_stats_variance(g1, 1, n1);
    double var2  = gsl_stats_variance(g2, 1, n2);
 
    *t_value = (mean1 - mean2) / sqrt(var1/n1 + var2/n2);
 
    double df = (var1/n1 + var2/n2) * (var1/n1 + var2/n2)
              / ((var1/n1)*(var1/n1)/(n1-1) + (var2/n2)*(var2/n2)/(n2-1));
    *p_value = 2 * gsl_cdf_tdist_Q(fabs(*t_value), df);
 
    free(g1);
    free(g2);
}

void chi_square(Dataset *dataset, int idx_x, int idx_y, double *chi_sq, double *p_value) {
    int rows, cols;
    FreqEntry *fx = frequencies(dataset, idx_x, &rows);
    FreqEntry *fy = frequencies(dataset, idx_y, &cols);

    double **observed = malloc(rows * sizeof(double *));
    double **expected = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        observed[i] = calloc(cols, sizeof(double));
        expected[i] = calloc(cols, sizeof(double));
    }

    for (int k = 0; k < dataset->num_cases; k++) {
        double vx = dataset->cases[k].values[idx_x];
        double vy = dataset->cases[k].values[idx_y];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (vx == fx[i].value && vy == fy[j].value) {
                    observed[i][j]++;
                }
            }
        }
    }

    int n = dataset->num_cases;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            expected[i][j] = (fx[i].count * fy[j].count) / (double)n;
        }
    }

    *chi_sq = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double diff = observed[i][j] - expected[i][j];
            *chi_sq += (diff * diff) / expected[i][j];
        }
    }

    double df = (rows - 1) * (cols - 1);
    *p_value = gsl_cdf_chisq_Q(*chi_sq, df);

    for (int i = 0; i < rows; i++) {
        free(observed[i]);
        free(expected[i]);
    }
    free(observed);
    free(expected);
    free(fx);
    free(fy);
}

void anova(Dataset *dataset, int idx_x, int idx_y, double *F_value, double *p_value) {
    int num_groups;
    FreqEntry *groups = frequencies(dataset, idx_x, &num_groups);

    double total_mean = mean(dataset, idx_y);

    double ss_between = 0;
    for (int g = 0; g < num_groups; g++) {
        int group_n = 0;
        double group_sum = 0;
        for (int k = 0; k < dataset->num_cases; k++) {
            if (dataset->cases[k].values[idx_x] == groups[g].value) {
                group_sum += dataset->cases[k].values[idx_y];
                group_n++;
            }
        }
        double group_mean = group_sum / group_n;
        ss_between += group_n * (group_mean - total_mean) * (group_mean - total_mean);
    }

    double ss_within = 0;
    for (int g = 0; g < num_groups; g++) {
        double group_sum = 0;
        int group_n = 0;
        for (int k = 0; k < dataset->num_cases; k++) {
            if (dataset->cases[k].values[idx_x] == groups[g].value) {
                group_sum += dataset->cases[k].values[idx_y];
                group_n++;
            }
        }
        double group_mean = group_sum / group_n;
        for (int k = 0; k < dataset->num_cases; k++) {
            if (dataset->cases[k].values[idx_x] == groups[g].value) {
                double diff = dataset->cases[k].values[idx_y] - group_mean;
                ss_within += diff * diff;
            }
        }
    }

    int n = dataset->num_cases;
    double df_between = num_groups - 1;
    double df_within = n - num_groups;
    *F_value = (ss_between / df_between) / (ss_within / df_within);

    *p_value = gsl_cdf_fdist_Q(*F_value, df_between, df_within);

    free(groups);
}

void paired_t_test(Dataset *dataset, int idx_x, int idx_y, double *t_value, double *p_value) {
    int n = dataset->num_cases;

    double *diff = malloc(n * sizeof(double));
    for (int j = 0; j < n; j++) {
        diff[j] = dataset->cases[j].values[idx_x] - dataset->cases[j].values[idx_y];
    }

    double mean_diff = gsl_stats_mean(diff, 1, n);
    double std_diff = gsl_stats_sd(diff, 1, n);

    *t_value = mean_diff / (std_diff / sqrt(n));

    double df = n - 1;
    *p_value = 2 * gsl_cdf_tdist_Q(fabs(*t_value), df);

    free(diff);
}