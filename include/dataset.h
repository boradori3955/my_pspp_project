#ifndef DATASET_H
#define DATASET_H

// 열 정보 (메타데이터)
typedef struct {
    char name[64];   // "age", "score" 등
    char type;       // 'N' = 숫자, 'S' = 문자열
} Variable;

// 행 데이터
typedef struct {
    double *values;  // values[0] = id, values[1] = age ...
    int num_values;
} Case;

// 전체 데이터셋
typedef struct {
    Variable *vars;  // 변수 목록
    int num_vars;
    Case *cases;     // 행 목록
    int num_cases;
    int capacity;
} Dataset;

void dataset_free(Dataset *dataset);

#endif