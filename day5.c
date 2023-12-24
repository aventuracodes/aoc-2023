#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_LINE 512
#define MAX_ITEMS 32
#define MAP_COUNT 7

// I'm not proud of this but by god it works...

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

static uint32_t map_range(FILE* input, uint32_t start, uint32_t count) {
    char line[MAX_LINE];

    uint32_t* items = malloc(sizeof(uint32_t) * count);
    for (uint32_t i = 0; i < count; i++) {
        items[i] = start + i;
    }

    uint8_t* mapped = malloc(count);
    memset(mapped, 0, count);

    rewind(input);
    read_line(input, line);
    read_line(input, line);

    for (int m = 0; m < MAP_COUNT; m++) {
        read_line(input, line);
        while (1) {
            int pos = 0;
            int len = read_line(input, line);
            if (!len) break;
            
            uint32_t dest = parse_number(line, &pos);
            pos++;
            
            uint32_t src = parse_number(line, &pos);
            pos++;

            uint32_t range = parse_number(line, &pos);

            for (uint32_t i = 0; i < count; i++) {
                if (!mapped[i] && items[i] >= src && items[i] <= (src + range - 1)) {
                    items[i] = dest + (items[i] - src);
                    mapped[i] = 1;
                }
            }
        }

        memset(mapped, 0, count);
    }

    uint32_t min = UINT32_MAX;
    for (uint32_t i = 0; i < count; i++) {
        if (items[i] < min) {
            min = items[i];
        }
    }

    free(items);
    free(mapped);

    return min;
}

int main(int argc, const char** argv) {
    if (argc != 2) {
        printf("Please pass exactly one file path\n!");
        return 0;
    }

    FILE* input = fopen(argv[1], "r");
    assert(input);

    uint32_t items[MAX_ITEMS];
    char line[MAX_LINE];

    int pos = 0;
    int item_count = 0;
    int len = read_line(input, line);
    while (pos < len) {
        if (!is_digit(line[pos])) {
            pos++;
        } else {
            uint32_t start = parse_number(line, &pos);
            pos++;

            uint32_t count = parse_number(line, &pos);
            pos++;

            items[item_count++] = map_range(input, start, count);
        }
    }

    uint32_t min = UINT32_MAX;
    for (int i = 0; i < item_count; i++) {
        if (items[i] < min) {
            min = items[i];
        }
    }

    printf("%"PRIu32"\n", min);
}