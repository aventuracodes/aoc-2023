#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 512
#define MAX_SYMBOLS 2048
#define MAX_NUMBERS 2048

struct number {
    int value;
    int x_start;
    int x_end;
    int y;
    int is_part_number;
};

struct symbol {
    int x, y;
    int is_star;
};

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

static int is_symbol(char c) {
    return c && (c != '.') && !is_digit(c) && (c != '\n');
}

int parse_number(char* line, int start, int y, struct number* number) {
    assert(is_digit(line[start]));

    int sum = 0;
    int end = start;
    while (1) {
        if (!is_digit(line[end])) {
            end--;
            break;
        }

        sum *= 10;
        sum += line[end] - '0';
        end++;
    }

    number->value = sum;
    number->x_start = start;
    number->x_end = end;
    number->y = y;
    number->is_part_number = 0;

    return end;
}

int read_line(FILE* file, char* line) {
    int len = 0;
    while(1) {
        assert(len < MAX_LINE);
        int c = fgetc(file);
        if (c < 0 || c == '\n') {
            return len;
        } else {
            line[len] = c;
            len++;
        }
    }
}

int next_to(int xns, int xne, int yn, int xs, int ys) {
    if (yn == ys) {
        return (xne == xs - 1) || (xns == xs + 1);
    } else if (yn == ys + 1 || yn == ys - 1) {
        return (xne >= xs - 1 && xne <= xs + 1) ||
               (xns >= xs - 1 && xns <= xs + 1);
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Please pass exactly one file path\n!");
        return 0;
    }

    FILE* input = fopen(argv[1], "r");
    assert(input);

    int line_len;
    char line[MAX_LINE];

    int number_count = 0;
    struct number numbers[MAX_NUMBERS];

    int symbol_count = 0;
    struct symbol symbols[MAX_SYMBOLS];

    // Parse input
    int y = 0;
    while((line_len = read_line(input, line))) {
        for (int x = 0; x < line_len; x++) {
            if (is_digit(line[x])) {
                assert(number_count < MAX_NUMBERS);
                x = parse_number(line, x, y, &numbers[number_count++]);
            } else if (is_symbol(line[x])) {
                assert(symbol_count < MAX_SYMBOLS);
                symbols[symbol_count++] = (struct symbol) {x, y, line[x] == '*'};
            }
        }
        y++;
    }

    // Find part numbers
    for (int i = 0; i < symbol_count; i++) {
        int x = symbols[i].x;
        int y = symbols[i].y;

        for (int j = 0; j < number_count; j++) {
            struct number* num = &numbers[j];
            if (next_to(num->x_start, num->x_end, num->y, x, y)) {
                num->is_part_number = 1;
            }
        }
    }

    // Find gears
    int sum = 0;
    for (int i = 0; i < symbol_count; i++) {
        if (!symbols[i].is_star) continue;

        int x = symbols[i].x;
        int y = symbols[i].y;

        int product = 1;
        int num_next_to = 0;
        for (int j = 0; j < number_count; j++) {
            struct number* num = &numbers[j];
            if (!num->is_part_number) continue;

            if (next_to(num->x_start, num->x_end, num->y, x, y)) {
                num_next_to += 1;
                product *= num->value;
            }
        }

        if (num_next_to == 2) {
            sum += product;
        }
    }

    printf("Sum %d\n", sum);
}