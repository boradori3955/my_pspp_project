CC = gcc
CFLAGS = -Wall -g -Iinclude
LIBS = -lgsl -lgslcblas -lm

SRCS = src/main.c src/csv_parser.c src/stats.c src/lexer.c src/parser.c src/output.c src/dataset.c
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

pspp: $(OBJS)
	$(CC) $(CFLAGS) -o pspp $(OBJS) $(LIBS)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build pspp