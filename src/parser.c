#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"
#include "parser.h"
#include "output.h"

int find_var_col(Dataset *dataset, const char *name) {
    for (int i = 0; i < dataset->num_vars; i++) {
        if (strcmp(dataset->vars[i].name, name) == 0) {
            return i;
        }
    }
    return -1;  // 못 찾으면 -1
}

void parser(TokenList *list, Dataset *dataset) {
    if (list->count == 0) return;

    if (strcmp(list->tokens[0], "DESCRIPTIVES") == 0) {
        int columns[64];
        int n = 0;

        for (int i = 1; i < list->count; i++) {
            if (strcmp(list->tokens[i], ".") == 0) break;
            
            int col = find_var_col(dataset, list->tokens[i]);
            if (col == -1) {
                fprintf(stderr, "변수를 찾을 수 없어요: %s\n", list->tokens[i]);
                continue;
            }
            columns[n++] = col;
        }

        print_descriptives(dataset, columns, n);
    }
    else if (strcmp(list->tokens[0], "FREQUENCIES") == 0) {
        for (int i = 1; i < list->count; i++) {
            if (strcmp(list->tokens[i], ".") == 0) break;

            int col = find_var_col(dataset, list->tokens[i]);
            if (col == -1) {
                fprintf(stderr, "변수를 찾을 수 없어요: %s\n", list->tokens[i]);
                continue;
            }

            print_frequencies(dataset, col);
        }
    }
    else if (strcmp(list->tokens[0], "INDEPENDENT-T-TEST") == 0) {
        if (list->count < 3) {
            fprintf(stderr, "INDEPENDENT-T-TEST는 변수 두 개가 필요해요\n");
            return;
        }

        int col_x = find_var_col(dataset, list->tokens[1]);
        int col_y = find_var_col(dataset, list->tokens[2]);

        if (col_x == -1 || col_y == -1) {
            fprintf(stderr, "변수를 찾을 수 없어요\n");
            return;
        }
        double t_value, p_value;
        independent_t_test(dataset, col_x, col_y, &t_value, &p_value);

        print_independent_ttest(dataset, col_x, col_y, t_value, p_value);
    }
    else if (strcmp(list->tokens[0], "PAIRED-T-TEST") == 0) {
        if (list->count < 3) {
            fprintf(stderr, "PAIRED-T-TEST는 변수 두 개가 필요해요\n");
            return;
        }

        int col_x = find_var_col(dataset, list->tokens[1]);
        int col_y = find_var_col(dataset, list->tokens[2]);

        if (col_x == -1 || col_y == -1) {
            fprintf(stderr, "변수를 찾을 수 없어요\n");
            return;
        }
        double t_value, p_value;
        paired_t_test(dataset, col_x, col_y, &t_value, &p_value);

        print_paired_ttest(dataset, col_x, col_y, t_value, p_value);
    }    
    else if (strcmp(list->tokens[0], "CHISQUARE") == 0) {
        if (list->count < 3) {
            fprintf(stderr, "CHISQUARE는 변수 두 개가 필요해요\n");
            return;
        }

        int col_x = find_var_col(dataset, list->tokens[1]);
        int col_y = find_var_col(dataset, list->tokens[2]);

        if (col_x == -1 || col_y == -1) {
            fprintf(stderr, "변수를 찾을 수 없어요\n");
            return;
        }
        double chi_sq, p_value;
        chi_square(dataset, col_x, col_y, &chi_sq, &p_value);

        print_chisquare(dataset, col_x, col_y, chi_sq, p_value);
    }    
    else if (strcmp(list->tokens[0], "ANOVA") == 0) {
        if (list->count < 3) {
            fprintf(stderr, "ANOVA는 변수 두 개가 필요해요\n");
            return;
        }

        int col_x = find_var_col(dataset, list->tokens[1]);
        int col_y = find_var_col(dataset, list->tokens[2]);

        if (col_x == -1 || col_y == -1) {
            fprintf(stderr, "변수를 찾을 수 없어요\n");
            return;
        }
        double F_value, p_value;
        anova(dataset, col_x, col_y, &F_value, &p_value);

        print_anova(dataset, col_x, col_y, F_value, p_value);
    }    
    else {
        printf("알 수 없는 명령어: %s\n", list->tokens[0]);
    }
}

