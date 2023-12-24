#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#define MAX_LINE 126
#define MAX_HAIL 512
#define TEST_MIN 200000000000000.0
#define TEST_MAX 400000000000000.0

struct hail {
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
};

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

uint64_t parse_u64(char* line, int* pos) {
    uint64_t num = 0;
    while (is_digit(line[*pos])) {
        num = (num * 10) + (uint64_t)(line[*pos] - '0');
        *pos += 1;
    }
    return num;
}

int parse_int(char* line, int* pos) {
    int sign = 1;
    if (line[*pos] == '-') {
        sign = -1;
        *pos += 1;
    }

    int num = 0;
    while (is_digit(line[*pos])) {
        num = (num * 10) + (int)(line[*pos] - '0');
        *pos += 1;
    }

    return num * sign;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    assert(input);

    char line[MAX_LINE];

    int hail_count = 0;
    struct hail hail[MAX_HAIL];
    
    while (1) {
        int pos = 0;
        int len = read_line(input, line);
        if (len == 0) break;

        struct hail* h = &hail[hail_count++];
        h->x = (double)parse_u64(line, &pos);
        pos += 2;
        
        h->y = (double)parse_u64(line, &pos);
        pos += 2;
        
        h->z = (double)parse_u64(line, &pos);
        pos += 3;

        h->vx = (double)parse_int(line, &pos);
        pos += 2;

        h->vy = (double)parse_int(line, &pos);
        pos += 2;

        h->vz = (double)parse_int(line, &pos);

        // printf("%.3f %.3f %.3f @ %.3f %.3f %.3f\n", h->x, h->y , h->z, h->vx, h->vy , h->vz);
    }

    int count = 0;
    for (int i = 0; i < hail_count; i++) {
        const struct hail* h1 = &hail[i]; 

        for (int j = i + 1; j < hail_count; j++) {
            const struct hail* h2 = &hail[j]; 

            // One page of math in my notebook later...
            double timea = ((h1->y - h2->y) + (h2->vy / h2->vx) * (h2->x - h1->x)) / ((h2->vy * h1->vx / h2->vx) - h1->vy);
            double timeb = (h1->x + (h1->vx * timea) - h2->x) / h2->vx;

            double x = h1->x + (h1->vx * timea);
            double y = h1->y + (h1->vy * timea);

            // printf("%.3f %.3f %.3f @ %.3f %.3f %.3f\n", h1->x, h1->y , h1->z, h1->vx, h1->vy , h1->vz);
            // printf("%.3f %.3f %.3f @ %.3f %.3f %.3f\n", h2->x, h2->y , h2->z, h2->vx, h2->vy , h2->vz);
            // printf("%f %f\n", timea, timeb);
            // printf("%f %f\n\n", x, y);

            if (
                timea > 0     &&
                timeb > 0     &&
                x >= TEST_MIN && 
                x <= TEST_MAX && 
                y >= TEST_MIN && 
                y <= TEST_MAX) {
                count++;
            }
        }
    }

    printf("%d\n", count);
}