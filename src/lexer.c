#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

TokenList lexer(const char *input) {
    TokenList list;
    list.count = 0;
    list.capacity = 16;
    list.tokens = malloc(list.capacity * sizeof(char *));

    char *copy = strdup(input);  // 원본 보호
    char *token = strtok(copy, " \t\n");  // 공백 또는 탭 또는 개행문자로 나누기
    while (token != NULL) {
        int len = strlen(token);
        if (token[len-1] == '.') {
            // 1. '.' 제거하고 앞부분 저장
            token[len-1] = '\0';
            if (len > 1) {
                list.tokens[list.count++] = strdup(token);
            }
            // 2. '.' 자체를 토큰으로 저장
            list.tokens[list.count++] = strdup(".");
        } else {
            list.tokens[list.count++] = strdup(token);
        }
        token = strtok(NULL, " \t\n");
    }

    free(copy);  // 원본 복사본 해제
    return list;
}