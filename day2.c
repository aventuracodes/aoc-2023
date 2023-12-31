#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14

#define LINE_MAX 256

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int parse_num(const char* buff, int* pos, int len) {
    int value = 0;
    
    while (is_digit(buff[*pos]) && *pos < len) {
        value *= 10;
        value += buff[*pos] - '0';
        *pos += 1;
    }

    return value;
}

int main(void) {
    FILE* input = fopen("input/day2.txt", "r");    
    assert(input);

    char buff[LINE_MAX];
    size_t buff_len = fread(buff, 1, LINE_MAX, input);

    int game = 1;
    int sum = 0;
    while (buff_len) {
        int line_len;
        for (int i = 0; i < LINE_MAX; i++) {
            if (buff[i] == '\n') {
                line_len = i + 1;
                break;
            }
        }

        int pos = 0;
        while (buff[pos++] != ':'); // First :
        pos++; // Skip space

        int max_r = 0;
        int max_g = 0;
        int max_b = 0;

        while (buff[pos] != '\n') {
            int value = parse_num(buff, &pos, line_len);
            pos++;

            switch (buff[pos]) {
                case 'r':
                    if (value > max_r) {
                        max_r = value;
                    }
                    break;
                case 'g':
                    if (value > max_g) {
                        max_g = value;
                    }
                    break;
                case 'b':
                    if (value > max_b) {
                        max_b = value;
                    }
                    break;
                default:
                    assert(0);
            }

            while (buff[pos] != '\n' && !is_digit(buff[pos])) {
                pos++;
            }
        }

        sum += (max_r * max_g * max_b);

        memmove(buff, buff + line_len, LINE_MAX - line_len);
        buff_len -= line_len;
        buff_len += fread(buff + (LINE_MAX - line_len), 1, line_len, input);
        game++;
    }

    printf("Sum: %d\n", sum);
}