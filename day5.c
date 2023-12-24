#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX_LINE 512
#define MAX_ITEMS 32
#define MAP_COUNT 7

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
    while (is_digit(number_str[*pos])) {
        number *= 10;
        number += (uint64_t)(number_str[*pos] - '0');
        *pos += 1;
    }
    return number;
}

int main(int argc, const char** argv) {
    if (argc != 2) {
        printf("Please pass exactly one file path\n!");
        return 0;
    }

    FILE* input = fopen(argv[1], "r");
    assert(input);

    uint64_t items[MAX_ITEMS];
    int mapped[MAX_ITEMS] = {0};
    char line[MAX_LINE];

    int pos = 0;
    int item_count = 0;
    int len = read_line(input, line);
    while (pos < len) {
        if (!is_digit(line[pos])) {
            pos++;
        } else {
            items[item_count++] = parse_number(line, &pos);
        }
    }

    read_line(input, line);

    for (int m = 0; m < MAP_COUNT; m++) {
        read_line(input, line);
        while (1) {
            pos = 0;
            len = read_line(input, line);
            if (!len) break;
            
            uint64_t dest = parse_number(line, &pos);
            pos++;
            
            uint64_t src = parse_number(line, &pos);
            pos++;

            uint64_t range = parse_number(line, &pos);

            for (int i = 0; i < item_count; i++) {
                if (!mapped[i] && items[i] >= src && items[i] <= (src + range - 1)) {
                    items[i] = dest + (items[i] - src);
                    mapped[i] = 1;
                }
            }
        }

        for (int i = 0; i < item_count; i++) {
            mapped[i] = 0;
        }
    }

    uint64_t min = UINT64_MAX;
    for (int i = 0; i < item_count; i++) {
        if (items[i] < min) {
            min = items[i];
        }
    }

    printf("%" PRIu64 "\n", min);
}