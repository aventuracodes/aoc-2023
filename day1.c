#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LINE_MAX 256

const char* digits[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

int main(void) {
    FILE* input = fopen("input/day1.txt", "r");    
    assert(input);

    char buff[LINE_MAX];
    size_t buff_len = fread(buff, 1, LINE_MAX, input);

    int sum = 0;
    while (1) {
        int line_len = 0;
        for (size_t i = 0; i < buff_len; i++) {
            if (buff[i] == '\n') {
                line_len = (int)(i + 1);
                break;
            }
        }

        for (int i = 0; i < line_len; i++) {
            if (buff[i] >= '0' && buff[i] <= '9') {
                sum += (buff[i] - '0') * 10;
                break;
            }

            int found_digit = 0;
            for (int j = 0; j < 9; j++) {
                int dig_len = strlen(digits[j]);
                if (memcmp(buff + i, digits[j], dig_len) == 0) {
                    sum += (j + 1) * 10;
                    found_digit = 1;
                    break;
                }
            }

            if (found_digit) {
                break;
            }
        }

        for (int i = line_len - 1; i >= 0; i--) {
            if (buff[i] >= '0' && buff[i] <= '9') {
                sum += (buff[i] - '0');
                break;
            }

            int found_digit = 0;
            for (int j = 0; j < 9; j++) {
                int dig_len = strlen(digits[j]);
                if (memcmp(buff + i, digits[j], dig_len) == 0) {
                    sum += j + 1;
                    found_digit = 1;
                    break;
                }
            }

            if (found_digit) {
                break;
            }
        }

        memmove(buff, buff + line_len, LINE_MAX - line_len);
        buff_len -= line_len;
        buff_len += fread(buff + (LINE_MAX - line_len), 1, line_len, input);
        
        if (buff_len == 0) {
            printf("Sum %d\n", sum);        
            return 0;
        }
    }
}