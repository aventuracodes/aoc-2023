#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_LINE 512
#define MAX_HANDS 1024



typedef struct {
    uint32_t bid;
    char cards[6];
} hand;

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

typedef enum {
    NO_PAIR,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
} hand_type;

const char cards[] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};

int count(char c, const char* cards) {
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (cards[i] == c) {
            count++;
        }
    }
    return count;
}

hand_type classify(const char* c) {
    if (count(c[0], c) == 5) {
        return FIVE_OF_A_KIND;
    }

    for (int i = 0; i < 2; i++) {
        if (count(c[i], c) == 4) {
            return FOUR_OF_A_KIND;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (count(c[i], c) == 3) {
            for (int j = 0; j < 5; j++) {
                if (c[i] != c[j] && count(c[j], c) == 2) {
                    return FULL_HOUSE;
                }
            }
            return THREE_OF_A_KIND;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (count(c[i], c) == 2) {
            for (int j = 0; j < 5; j++) {
                if (c[i] != c[j] && count(c[j], c) == 2) {
                    return TWO_PAIR;
                }
            }
            return ONE_PAIR;
        }
    }

    return NO_PAIR;
}

int break_tie(const char* a, const char* b) {
    for (int i = 0; i < 5; i++) {
        if (a[i] != b[i]) {
            for (int c = 0; c < sizeof(cards); c++) {
                if (a[i] == cards[c]) {
                    return 1;
                } else if (b[i] == cards[c]) {
                    return -1;
                }
            }
        }
    }

    assert(0);
    return 0;
}

int hand_sort(const void* a, const void* b) {
    const hand* hand1 = (const hand*)a;
    const hand* hand2 = (const hand*)b;

    hand_type ht1 = classify(hand1->cards);
    hand_type ht2 = classify(hand2->cards);

    if (ht1 > ht2) {
        return 1;
    } else if (ht2 > ht1) {
        return -1;
    } else {
        return break_tie(hand1->cards, hand2->cards);
    }

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

    uint32_t hand_count = 0;
    hand hands[MAX_HANDS] = {0};

    while (1) {
        int len = read_line(input, line);
        if (len == 0) break;

        memcpy(hands[hand_count].cards, line, 5);
        int pos = 6;
        hands[hand_count].bid = parse_number(line, &pos);

        hand_count++;
    }

    qsort(hands, hand_count, sizeof(hand), hand_sort);

    uint32_t total = 0;
    for (uint32_t i = 0; i < hand_count; i++) {
        total += hands[i].bid * (i + 1);
    }

    printf("%"PRIu32"\n", total);
}