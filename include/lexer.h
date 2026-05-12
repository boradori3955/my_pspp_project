#ifndef LEXER_H
#define LEXER_H

typedef struct {
    char **tokens;   // 토큰 배열
    int count;       // 토큰 개수
    int capacity;    // 현재 할당된 공간
} TokenList;

TokenList lexer(const char *input);

#endif