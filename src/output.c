#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "output.h"
#include "stats.h"

void print_separator(int name_width) {
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+--------+--------+--------+--------+--------+--------+--------+--------+\n");
}

void print_header(int name_width) {
    printf("| %-*s | %6s | %6s | %6s | %6s | %6s | %6s | %6s | %6s |\n",
        name_width, "Variable", "Mean", "StdDev", "Min", "Max", "Median", "Q1", "Q2", "Q3");
}

void print_row(int name_width, char *name, double mean, double stddev,
               double min, double max, double median, double q1, double q2,  double q3) {
    printf("| %-*s | %6.2f | %6.2f | %6.2f | %6.2f | %6.2f | %6.2f | %6.2f | %6.2f |\n",
        name_width, name, mean, stddev, min, max, median, q1, q2, q3);
}

void print_descriptives(Dataset *dataset, int *columns, int n) {
    // 변수 이름 중 제일 긴 길이 계산
    int max_len = 8;  // "Variable" 길이
    for (int i = 0; i < n; i++) {
        int len = strlen(dataset->vars[columns[i]].name);
        if (len > max_len) max_len = len;
    }

    print_separator(max_len);
    print_header(max_len);
    print_separator(max_len);

    for (int i = 0; i < n; i++) {
        int col = columns[i];
        double q1, q2, q3;
        quartiles(dataset, col, &q1, &q2, &q3);        
        print_row(max_len, dataset->vars[col].name, mean(dataset, col), stddev(dataset, col), min_val(dataset, col), max_val(dataset, col), median(dataset, col), q1, q2, q3);
    }

    print_separator(max_len);
}

void print_frequencies(Dataset *dataset, int col) {
    int size = 0;
    FreqEntry *freq = frequencies(dataset, col, &size);

    printf("변수: %s\n", dataset->vars[col].name);
    printf("+-------+-------+\n");
    printf("| %5s | %5s |\n", "Value", "Count");
    printf("+-------+-------+\n");
    for (int j = 0; j < size; j++) {
        printf("| %5.2f | %5d |\n", freq[j].value, freq[j].count);
    }
    printf("+-------+-------+\n");

    free(freq);
}

void print_independent_ttest(Dataset *dataset, int col_x, int col_y, double t_value, double p_value) {
    char label[256];
    snprintf(label, sizeof(label), "%s vs %s", dataset->vars[col_x].name, dataset->vars[col_y].name);    
    
    int label_len = strlen(label);
    if (label_len < 8) label_len = 8;

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+----------+---------+\n");    

    printf("| %-*s | %8s | %7s |\n", label_len, "Variable", "t-value", "p-value");

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+----------+---------+\n");

    printf("| %-*s | %8.4f | %7.4f |\n",label_len, label, t_value, p_value);

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+----------+---------+\n");
}

void print_chisquare(Dataset *dataset, int col_x, int col_y, double chi_sq, double p_value) {
    char label[256];
    snprintf(label, sizeof(label), "%s vs %s", dataset->vars[col_x].name, dataset->vars[col_y].name);

    int label_len = strlen(label);
    if (label_len < 8) label_len = 8;

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+------------+---------+\n");

    printf("| %-*s | %10s | %7s |\n", label_len, "Variable", "chi-square", "p-value");

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+------------+---------+\n");

    printf("| %-*s | %10.4f | %7.4f |\n", label_len, label, chi_sq, p_value);

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+------------+---------+\n");
}

void print_anova(Dataset *dataset, int col_x, int col_y, double F_value, double p_value) {
    char label[256];
    snprintf(label, sizeof(label), "%s vs %s", dataset->vars[col_x].name, dataset->vars[col_y].name);

    int label_len = strlen(label);
    if (label_len < 8) label_len = 8;

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+------------+---------+\n");

    printf("| %-*s | %10s | %7s |\n", label_len, "Variable", "F-value", "p-value");

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+------------+---------+\n");

    printf("| %-*s | %10.4f | %7.4f |\n", label_len, label, F_value, p_value);

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+------------+---------+\n");
}

void print_paired_ttest(Dataset *dataset, int col_x, int col_y, double t_value, double p_value) {
    char label[256];
    snprintf(label, sizeof(label), "%s vs %s", dataset->vars[col_x].name, dataset->vars[col_y].name);    
    
    int label_len = strlen(label);
    if (label_len < 8) label_len = 8;

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+----------+---------+\n");    

    printf("| %-*s | %8s | %7s |\n", label_len, "Variable", "t-value", "p-value");

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+----------+---------+\n");

    printf("| %-*s | %8.4f | %7.4f |\n",label_len, label, t_value, p_value);

    printf("+");
    for (int i = 0; i < label_len + 2; i++) printf("-");
    printf("+----------+---------+\n");
}