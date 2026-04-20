#include "lexer.h"
#include <string.h>
#include <ctype.h>

T_Type get_keyword_type(char* value) {
    // 1. முதன்மை / Main
    if (strcmp(value, "முதன்மை") == 0 || strcmp(value, "main") == 0) {
        return T_MAIN;
    }
    // 2. நிகழ் / fun
    if (strcmp(value, "நிகழ்") == 0 || strcmp(value, "fun") == 0) {
        return T_FUN;
    }
    // 3. அச்சிடு / print
    if (strcmp(value, "அச்சிடு") == 0 || strcmp(value, "print") == 0) {
        return T_PRINT;
    }
    // 4. உள்ளீடு / input
    if (strcmp(value, "உள்ளீடு") == 0 || strcmp(value, "input") == 0) {
        return T_INP;
    }
    // 5. சேர் / import
    if (strcmp(value, "சேர்") == 0 || strcmp(value, "import") == 0) {
        return T_IMP;
    }
    // 6. முழுஎண் / Num (முக்கியமானது!)
    if (strcmp(value, "முழுஎண்") == 0 || strcmp(value, "Num") == 0) {
        return T_INT;
    }
    // 7. Str
    if (strcmp(value, "Str") == 0) return T_STR;
    // 8. if
    if (strcmp(value, "if") == 0) return T_IF;
    // 9. சு / for
    if (strcmp(value, "சு") == 0 || strcmp(value, "for") == 0) {
        return T_FOR;
    }
    // 10. சு2 / while
    if (strcmp(value, "சு2") == 0 || strcmp(value, "while") == 0) {
        return T_WHILE;
    }
    // 11. return / bool / call
    if (strcmp(value, "return") == 0) return T_RET;
    if (strcmp(value, "bool") == 0) return T_BOOL;
    if (strcmp(value, "இயக்கு") == 0 || strcmp(value, "call") == 0) {
        return T_CALL;
    }

    return T_ID; 
}

Token get_next_token(FILE *file) {
    Token token;
    int c = fgetc(file);

    while (isspace(c)) c = fgetc(file);

    if (c == EOF) {
        token.type = T_EOF;
        strcpy(token.value, "EOF");
        return token;
    }

    // தமிழ் மற்றும் ஆங்கில எழுத்துக்களைக் கையாளுதல்
    if (isalpha(c) || (unsigned char)c > 127) {
        int i = 0;
        do {
            token.value[i++] = c;
            c = fgetc(file);
        } while (isalnum(c) || (unsigned char)c > 127 || c == '2'); 
        ungetc(c, file);
        token.value[i] = '\0';
        token.type = get_keyword_type(token.value);
        return token;
    }

    // எண்களைக் கையாளுதல்
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

    // குறியீடுகள் (Symbols)
    token.value[0] = c;
    token.value[1] = '\0';

    if (c == '(') token.type = 15;
    else if (c == ')') token.type = 16;
    else if (c == ';') token.type = 17;
    else if (c == '<') token.type = 18;
    else if (c == '>') token.type = 21;
    else if (c == '+') token.type = 19;
    else if (c == '=') token.type = 20;
    else if (c == '{') token.type = 22;
    else if (c == '}') token.type = 23;
    else token.type = T_ID;

    return token;
}
