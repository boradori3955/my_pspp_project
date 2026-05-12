# my_pspp_project

GNU PSPP 통계 분석 프로그램의 핵심 기능을 C언어로 직접 구현한 프로젝트입니다.  
CSV 파일을 읽어 명령어 기반으로 다양한 통계 분석을 수행합니다.

---

## 빌드 방법

### 의존성 설치
```bash
sudo apt install libgsl-dev
```

### 빌드
```bash
make
```

### 실행
```bash
./pspp data.csv
```

---

## 사용법

프로그램을 실행하면 명령어를 입력할 수 있는 프롬프트가 열립니다.

```
데이터 로드 완료: 5개 변수, 10개 케이스
DESCRIPTIVES age score.
...
EXIT.
```

> 명령어는 반드시 `.`으로 끝나야 합니다.

---

## 지원 명령어

| 명령어 | 설명 | 예시 |
|--------|------|------|
| `DESCRIPTIVES` | 기술 통계 (평균, 표준편차, 최솟값, 최댓값, 중앙값, 사분위수) | `DESCRIPTIVES age score.` |
| `FREQUENCIES` | 빈도 분석 | `FREQUENCIES grade gender region.` |
| `INDEPENDENT-T-TEST` | 독립 t-검정 (두 집단 평균 비교) | `INDEPENDENT-T-TEST group score.` |
| `PAIRED-T-TEST` | 대응 t-검정 (전후 비교) | `PAIRED-T-TEST before after.` |
| `CHISQUARE` | 카이제곱 검정 (범주형 변수 간 관계) | `CHISQUARE sex preference.` |
| `ANOVA` | 일원분산분석 (집단 간 평균 비교) | `ANOVA group score.` |
| `EXIT` | 프로그램 종료 | `EXIT.` |

---

## 출력 예시

### DESCRIPTIVES
```
./pspp descriptives_data.csv
데이터 로드 완료: 5개 변수, 10개 케이스
DESCRIPTIVES age score height weight.
+----------+--------+--------+--------+--------+--------+--------+--------+--------+
| Variable |   Mean | StdDev |    Min |    Max | Median |     Q1 |     Q2 |     Q3 |
+----------+--------+--------+--------+--------+--------+--------+--------+--------+
| age      |  29.50 |   5.20 |  22.00 |  40.00 |  29.00 |  25.00 |  30.00 |  33.00 |
| score    |  85.20 |   6.01 |  76.00 |  95.00 |  86.00 |  79.00 |  87.00 |  90.00 |
| height   | 171.70 |   4.61 | 165.00 | 180.00 | 171.00 | 168.00 | 172.00 | 175.00 |
| weight   |  68.40 |   5.89 |  60.00 |  80.00 |  68.50 |  63.00 |  69.00 |  72.00 |
+----------+--------+--------+--------+--------+--------+--------+--------+--------+
```

### FREQUENCIES
```
./pspp frequencies_data.csv
데이터 로드 완료: 4개 변수, 12개 케이스
FREQUENCIES grade gender region.
변수: grade
+-------+-------+
| Value | Count |
+-------+-------+
|  1.00 |     5 |
|  2.00 |     4 |
|  3.00 |     3 |
+-------+-------+
변수: gender
+-------+-------+
| Value | Count |
+-------+-------+
|  1.00 |     6 |
|  2.00 |     6 |
+-------+-------+
변수: region
+-------+-------+
| Value | Count |
+-------+-------+
|  1.00 |     5 |
|  2.00 |     4 |
|  3.00 |     3 |
+-------+-------+
```

### INDEPENDENT-T-TEST
```
./pspp independent_ttest_data.csv
데이터 로드 완료: 3개 변수, 20개 케이스
INDEPENDENT-T-TEST group score.
+----------------+----------+---------+
| Variable       |  t-value | p-value |
+----------------+----------+---------+
| group vs score |  -2.2947 |  0.0340 |
+----------------+----------+---------+
```

### PAIRED-T-TEST
```
./pspp paired_ttest_data.csv
데이터 로드 완료: 3개 변수, 10개 케이스
PAIRED-T-TEST before after.
+-----------------+----------+---------+
| Variable        |  t-value | p-value |
+-----------------+----------+---------+
| before vs after |   3.1429 |  0.0119 |
+-----------------+----------+---------+
```

### CHISQUARE
```
./pspp chisquare_data.csv
데이터 로드 완료: 3개 변수, 40개 케이스
CHISQUARE sex preference.
+-------------------+------------+---------+
| Variable          | chi-square | p-value |
+-------------------+------------+---------+
| sex vs preference |     7.5556 |  0.0229 |
+-------------------+------------+---------+
```

### ANOVA
```
./pspp anova_data.csv
데이터 로드 완료: 3개 변수, 24개 케이스
ANOVA group score.
+----------------+------------+---------+
| Variable       |    F-value | p-value |
+----------------+------------+---------+
| group vs score |     4.5006 |  0.0236 |
+----------------+------------+---------+
```

---

## 프로젝트 구조

```
my_pspp_project/
├── src/
│   ├── main.c          # 진입점, 명령어 루프
│   ├── csv_parser.c    # CSV 파일 파서
│   ├── dataset.c       # 데이터셋 메모리 관리
│   ├── lexer.c         # 명령어 토큰화
│   ├── parser.c        # 명령어 파싱 및 실행
│   ├── stats.c         # 통계 연산 엔진
│   └── output.c        # 결과 출력 포맷터
├── include/
│   ├── csv_parser.h
│   ├── dataset.h
│   ├── lexer.h
│   ├── parser.h
│   ├── stats.h
│   └── output.h
├── build/              # 빌드 결과물 (.o 파일)
└── Makefile
```

---

## 구현 내용

### 데이터 구조
- `Variable` - 열 이름, 타입 저장
- `Case` - 한 행의 데이터를 동적 배열로 저장
- `Dataset` - 전체 데이터셋 (동적 배열, realloc으로 확장)

### 명령어 파서
- **Lexer** - 입력 문자열을 공백/`.` 기준으로 토큰화
- **Parser** - 토큰을 해석해 해당 통계 함수 호출

### 통계 엔진
| 함수 | 설명 |
|------|------|
| `mean()` | 평균 |
| `stddev()` | 표준편차 |
| `min_val()` / `max_val()` | 최솟값 / 최댓값 |
| `median()` | 중앙값 |
| `quartiles()` | 사분위수 (Q1, Q2, Q3) |
| `frequencies()` | 빈도 분석 |
| `correlation()` | 상관계수 |
| `independent_t_test()` | 독립 t-검정 |
| `paired_t_test()` | 대응 t-검정 |
| `chi_square()` | 카이제곱 검정 |
| `anova()` | 일원분산분석 |

---

## 메모리 관리

`valgrind`로 메모리 누수 검사를 완료했습니다.

```bash
valgrind --leak-check=full ./pspp data.csv
# All heap blocks were freed -- no leaks are possible
```
