#include "parser.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// புதுசா ஒரு Statement Parser தேவை, அப்போதான் nested logic வேலை செய்யும்
void parse_statement(FILE *file, Token t);

void parse(FILE *file) {
    Token t;
    fprintf(stderr, "[Parser] Tamizhi Engine Analysis Started...\n");

    // மெயின் லூப் - ஒவ்வொரு ஸ்டேட்மென்டா எடுக்கும்
    while ((t = get_next_token(file)).type != T_EOF) {
        parse_statement(file, t);
    }
}

void parse_statement(FILE *file, Token t) {
    // --- 1. 'Num' / 'முழுஎண்' (Variables) ---
    if (t.type == T_INT || strcmp(t.value, "Num") == 0 || strcmp(t.value, "முழுஎண்") == 0) {
        Token name_token = get_next_token(file); 
        while ((t = get_next_token(file)).type != 20 && t.type != T_EOF); // '=' தேடு
        Token first_val = get_next_token(file); 
        Token next_t = get_next_token(file);

        if (next_t.type == 19 || strcmp(next_t.value, "+") == 0) {
            Token second_val = get_next_token(file);
            tamizhi_gen_var_add(name_token.value, first_val.value, second_val.value);
        } else {
            if (isdigit(first_val.value[0])) {
                tamizhi_gen_var(name_token.value, atoi(first_val.value));
            }
        }
    }

    // --- 2. 'அச்சிடு' (Print) ---
    else if (t.type == T_PRINT || strcmp(t.value, "அச்சிடு") == 0) {
        Token var_t = get_next_token(file);
        if (var_t.type == 15) var_t = get_next_token(file); // '(' தாண்டு
        tamizhi_gen_print(var_t.value);
    }

    // --- 3. 'சு' (Loop) ---
    else if (t.type == T_FOR || strcmp(t.value, "சு") == 0) {
        while ((t = get_next_token(file)).type != T_NUM && t.type != T_EOF);
        int limit = atoi(t.value);
        tamizhi_gen_loop_start(limit); // லூப் தொடக்கம்

        while ((t = get_next_token(file)).type != 22 && t.type != T_EOF); // '{' தேடு
        
        // பிளாக்குக்குள்ள இருக்குற கோடை படிக்க Recursion பயன்படுத்துகிறோம்
        while ((t = get_next_token(file)).type != 23 && t.type != T_EOF) { // '}' வரும் வரை
            parse_statement(file, t);
        }
        tamizhi_gen_loop_end(); // லூப் முடிவு
    }

    // --- 4. 'எனில்' (If Condition - NEW!) ---
    else if (t.type == T_IF || strcmp(t.value, "எனில்") == 0) {
        while ((t = get_next_token(file)).type != 15 && t.type != T_EOF); // '(' தேடு
        Token var1 = get_next_token(file);
        Token op = get_next_token(file);
        Token var2 = get_next_token(file);
        
        tamizhi_gen_if_start(var1.value, op.value, var2.value);

        while ((t = get_next_token(file)).type != 22 && t.type != T_EOF); // '{' தேடு
        
        // If-Block உள்ளே இருக்கும் கோடை படிக்கிறோம்
        while ((t = get_next_token(file)).type != 23 && t.type != T_EOF) {
            parse_statement(file, t);
        }
        
        // 'இல்லையெனில்' (Else) இருக்கான்னு பார்க்கணும்
        Token next_look = get_next_token(file);
        if (strcmp(next_look.value, "else") == 0 || strcmp(next_look.value, "இல்லையெனில்") == 0) {
            tamizhi_gen_else_start();
            while ((t = get_next_token(file)).type != 22 && t.type != T_EOF); // '{'
            while ((t = get_next_token(file)).type != 23 && t.type != T_EOF) {
                parse_statement(file, t);
            }
        }
        tamizhi_gen_if_end();
    }
}
