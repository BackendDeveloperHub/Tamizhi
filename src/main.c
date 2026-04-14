#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Itha extra-va add pannunga (isspace-ku thevai)

// 1. Enums (Already iruku)
typedef enum {
    T_INT, T_STR, T_FLT, T_BOOL,      // எ, வ, பு, உ
    T_IF, T_ELSE, T_FOR, T_WHILE,     // ஆ2, இ, சு, சு2
    T_RET, T_FUNC, T_PRINT,           // த, செயல், கூறு
    T_INP, T_IMP, T_ID, T_NUM, T_EOF  // கேள், சேர், identifiers, end of file
} T_Type;


// 2. Token Struct (Puthusa paste pannunga)
typedef struct {
    T_Type type;
    char value[64];
} Token;

// 3. Keyword Helper (Puthusa paste pannunga)
T_Type get_keyword_type(char* value) {
    if (strcmp(value, "செயல்") == 0) return T_FUNC;
    if (strcmp(value, "கூறு") == 0) return T_PRINT;
    if (strcmp(value, "கேள்") == 0) return T_INP;
    if (strcmp(value, "சேர்") == 0) return T_IMP;
    if (strcmp(value, "எ") == 0) return T_INT;
    if (strcmp(value, "ஆ2") == 0) return T_IF;
    if (strcmp(value, "த") == 0) return T_RET;
    return T_ID; 
}

// 4. Advanced Lexer Function (Puthusa paste pannunga)
Token get_next_token(FILE *file) {
    Token token;
    int c = fgetc(file);
    while (isspace(c)) c = fgetc(file); // Spaces skip panna

    if (c == EOF) {
        token.type = T_EOF;
        return token;
    }

    // Unicode & Identifier handling
    Token get_next_token(FILE *file) {
    Token token;
    int c = fgetc(file);
    while (isspace(c)) c = fgetc(file);

    if (c == EOF) {
        token.type = T_EOF;
        return token;
    }

    // 1. Identifiers & Tamil Keywords (Already neenga vachiruping)
    if (isalpha(c) || c > 127) {
        int i = 0;
        do {
            token.value[i++] = c;
            c = fgetc(file);
        } while (isalnum(c) || c > 127 || c == '2'); // '2' ah allow panna (ஆ2, சு2)
        ungetc(c, file);
        token.value[i] = '\0';
        token.type = get_keyword_type(token.value);
        return token;
    }

    // 2. Symbols Handling (Ippo neenga ketta logic - Itha thaan add pannanum)
    token.value[0] = c;
    token.value[1] = '\0';

    if (c == '=') { token.type = T_ID; return token; }
    if (c == ';') { token.type = T_ID; return token; }
    if (c == '(') { token.type = T_ID; return token; }
    if (c == ')') { token.type = T_ID; return token; }
    if (c == '{') { token.type = T_ID; return token; }
    if (c == '}') { token.type = T_ID; return token; }

    // 3. Numbers Handling
    if (isdigit(c)) {
        int i = 0;
        while (isdigit(c)) {
            token.value[i++] = c;
            c = fgetc(file);
        }
        ungetc(c, file);
        token.value[i] = '\0';
        token.type = T_NUM;
        return token;
    }

    return token;
}

    // Symbols handling (Ithaiyum add pannunga)
    if (c == '=') {
        token.type = T_ID; // Temporary-ah assign panrom
        strcpy(token.value, "=");
        return token;
    }

    return token;
}

// 5. Main Function (Itha update pannunga)
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
    
    // நாம் எழுதிய புதிய get_next_token ஃபங்ஷனை இங்கே பயன்படுத்துகிறோம்
    while ((t = get_next_token(file)).type != T_EOF) {
        printf("வகை: %d | மதிப்பு: %s\n", t.type, t.value);
    }

    fclose(file);
    printf("\nஆய்வு முடிந்தது.\n");
    return 0;
}
