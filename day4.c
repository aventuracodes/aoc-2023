#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_LINE 512
#define MAX_NUMBERS 64
#define MAX_GAMES 256

static int read_line(FILE* file, char* line, int line_size) {
    int len = 0;
    while(1) {
        assert(len < line_size);
        int c = fgetc(file);
        if (c < 0 || c == '\n') {
            line[len + 1] = 0;
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

static int parse_number(char* line, int* pos) {
    int sum = 0;

    while(is_digit(line[*pos])) {
        sum *= 10;
        sum += line[*pos] - '0';
        *pos += 1;
    }

    return sum;
}

static int parse_game(char* line) {
    // Get to the :
    int pos = 0;
    while(1) {
        assert(line[pos]);
        if (line[pos++] == ':') {
            break;
        }
    }

    // Parse winning numbers
    int number_count = 0;
    int numbers[MAX_NUMBERS];

    while(line[pos] != '|') {
        if (is_digit(line[pos])) {
            assert(number_count < MAX_NUMBERS);
            numbers[number_count++] = parse_number(line, &pos);
        } else {
            pos++;
        }
    }

    // Calculate score
    int score = 0;

    while(line[pos]) {
        if (!is_digit(line[pos])) {
            pos++;
            continue;
        }

        int num = parse_number(line, &pos);
        for (int i = 0; i < number_count; i++) {
            if (numbers[i] == num) {
                score++;
            }
        }
    }

    return score;
}

int count_cards(int* scores, int game) {
    int count = scores[game];
    for (int i = 0; i < scores[game]; i++) {
        count += count_cards(scores, game + 1 + i);
    }
    return count;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Please pass exactly one file path\n!");
        return 0;
    }

    FILE* input = fopen(argv[1], "r");
    assert(input);

    char line[MAX_LINE];

    int score_count = 0;
    int scores[MAX_GAMES];
    while (read_line(input, line, MAX_LINE)) {
        assert(score_count < MAX_GAMES);
        scores[score_count++] = parse_game(line);
    }

    int sum = score_count;
    for (int i = 0; i < score_count; i++) {
        sum += count_cards(scores, i);
    }

    printf("Sum: %d\n", sum);
}