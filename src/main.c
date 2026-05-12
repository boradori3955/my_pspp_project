#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"
#include "dataset.h"
#include "lexer.h"
#include "parser.h"
#include "stats.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: ./pspp data.csv\n");
        return 1;
    }

    // 1. CSV 파일 읽기
    Dataset dataset;
    parse_csv(&dataset, argv[1]);

    printf("데이터 로드 완료: %d개 변수, %d개 케이스\n",
        dataset.num_vars, dataset.num_cases);

    // 2~4. 명령어 반복 처리
    char input[1024];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        // "EXIT." 입력하면 종료
        if (strncmp(input, "EXIT", 4) == 0) break;

        // Lexer
        TokenList list = lexer(input);       

        // Parser
        parser(&list, &dataset);

        // 메모리 해제
        for (int i = 0; i < list.count; i++) {
            free(list.tokens[i]);
        }
        free(list.tokens);
    }
    dataset_free(&dataset);
    return 0;
}

