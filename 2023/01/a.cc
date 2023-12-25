#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int process(char *line) {
    int first = -1;
    int last = -1;
    for (char c = *line; c != '\0'; c = *++line) {
        if (isdigit(c)) {
            if (first == -1) {
                first = atoi(&c);
            } else {
                last = atoi(&c);
            }
        }
    }
    if (last == -1) {
        last = first;
    }
    int num = first*10 + last;
    printf("%d\n", num);
    return num;
}

int main() {
    int ans = 0;

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("./input.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s", line);
        ans += process(line);
    }

    printf("Ans: %d\n", ans);

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}