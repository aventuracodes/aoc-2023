#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_LINE 512
#define MAX_RACES 32

typedef struct {
    uint32_t time;
    uint32_t distance;
} race;

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

static uint32_t parse_number(char* number_str, int* pos) {
    uint32_t number = 0;
    while (is_digit(number_str[*pos])) {
        number *= 10;
        number += (uint32_t)(number_str[*pos] - '0');
        *pos += 1;
    }
    return number;
}

int main(int argc, char** argv) {
    FILE* input = fopen("input.txt", "r");
    assert(input);

    char line[MAX_LINE];
    race races[MAX_RACES];

    int race_count = 0;
    int pos = 0;
    int len = read_line(input, line);
    while (pos < len) {
        if (!is_digit(line[pos])) {
            pos++;
        } else {
            races[race_count++].time = parse_number(line, &pos);
        }
    }

    race_count = 0;
    pos = 0;
    len = read_line(input, line);
    while (pos < len) {
        if (!is_digit(line[pos])) {
            pos++;
        } else {
            races[race_count++].distance = parse_number(line, &pos);
        }
    }

    uint32_t margin = 1;
    for (uint32_t r = 0; r < race_count; r++) {
        uint32_t number = 0;
        for (uint32_t t = 0; t < races[r].time; t++) {
            if ((t * (races[r].time - t)) > races[r].distance) {
                number++;
            }
        }
        margin *= number;
    }

    printf("%"PRIu32"\n", margin);
}