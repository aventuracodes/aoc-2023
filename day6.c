#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_LINE 512
#define MAX_RACES 32

static int read_line(FILE* file, char* line) {
    int len = 0;
    while(1) {
        assert(len < MAX_LINE);
        int c = fgetc(file);
        if (c < 0 || c == '\n') {
            line[len] = 0;
            return len;
        } else {
            line[len] = c;
            len++;
        }
    }
}

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

static uint64_t parse_number(char* number_str, int* pos) {
    uint64_t number = 0;
    while (is_digit(number_str[*pos]) || number_str[*pos] == ' ') {
        if (number_str[*pos] != ' ') {
            number *= 10;
            number += (uint64_t)(number_str[*pos] - '0');
        }
        *pos += 1;
    }
    return number;
}

int main(int argc, char** argv) {
    FILE* input = fopen("input.txt", "r");
    assert(input);

    char line[MAX_LINE];
    uint64_t time;
    uint64_t distance;

    int race_count = 0;
    int pos = 0;
    int len = read_line(input, line);
    while (pos < len) {
        if (!is_digit(line[pos])) {
            pos++;
        } else {
            time = parse_number(line, &pos);
            break;
        }
    }

    race_count = 0;
    pos = 0;
    len = read_line(input, line);
    while (pos < len) {
        if (!is_digit(line[pos])) {
            pos++;
        } else {
            distance = parse_number(line, &pos);
            break;
        }
    }

    printf("%"PRIu64" %"PRIu64" \n", time, distance);

    uint64_t number = 0;
    for (uint64_t t = 0; t < time; t++) {
        if ((t * (time - t)) > distance) {
            number++;
        }
    }

    printf("%"PRIu64"\n", number);
}