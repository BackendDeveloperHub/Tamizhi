#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("பயன்பாடு: tamizhi <filename.tz>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("கோப்பை திறக்க முடியவில்லை");
        return 1;
    }

    Token t;
    printf("--- தமிழி கம்பைலர் (v0.1) ---\n");

    while ((t = get_next_token(file)).type != T_EOF) {
        printf("வகை: %d | மதிப்பு: %s\n", t.type, t.value);
    }

    fclose(file);
    printf("\nஆய்வு முடிந்தது.\n");
    return 0;
}
