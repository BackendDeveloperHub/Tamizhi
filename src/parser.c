#include "parser.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void skip_block(FILE *file); 

void parse(FILE *file) {
    Token t;
    fprintf(stderr, "[Parser] Tamizhi Engine Analysis Started...\n");

    while ((t = get_next_token(file)).type != T_EOF) {

        // 1. 'Num' அல்லது 'முழுஎண்'
        if (t.type == T_INT || strcmp(t.value, "Num") == 0 || strcmp(t.value, "முழுஎண்") == 0) {
            Token name_token = get_next_token(file); 
            
            // '=' வரும் வரை தேடுவோம்
            while ((t = get_next_token(file)).type != 20 && t.type != T_EOF); 

            Token first_val = get_next_token(file); 
            
            // --- அட்வான்ஸ்டு டோக்கன் செக் ---
            // அடுத்த டோக்கனைப் பார்ப்போம், அது '+' தானா என்று
            Token next_t = get_next_token(file);
            
            if (next_t.type == 19 || strcmp(next_t.value, "+") == 0) {
                // இது கூட்டல்: Num c = a + b
                Token second_val = get_next_token(file);
                tamizhi_gen_var_add(name_token.value, first_val.value, second_val.value);
            } else {
                // இது சாதாரண அசைன்மென்ட்: Num p = 100
                if (isdigit(first_val.value[0])) {
                    tamizhi_gen_var(name_token.value, atoi(first_val.value));
                }
                // *** மிக முக்கியம்: *** // எடுத்த 'next_t' ஒருவேளை 'அச்சிடு' ஆக இருந்தால், 
                // அதை மீண்டும் ஃபைலுக்குள்ளேயே தள்ள வேண்டும் (Push back)
                // அல்லது அந்த டோக்கனை இங்கேயே கையாள வேண்டும்.
                // தற்காலிக தீர்வு: next_t-ஐ செக் பண்ணி அது அச்சிடு-வா என்று பார்ப்போம்.
                if (next_t.type == T_PRINT || strcmp(next_t.value, "அச்சிடு") == 0) {
                     // இப்போதைக்கு இதை அப்படியே லூப்புக்கு விடாமல் இங்கேயே ஹேண்டில் பண்ணலாம்
                     // (அல்லது லெக்சர்ல unget_token பங்க்ஷன் இருந்தால் அதை பயன்படுத்தலாம்)
                }
            }
            continue;
        }

        // 2. 'அச்சிடு' - இது இப்போ கிளீனா வேலை செய்யும்
        else if (t.type == T_PRINT || strcmp(t.value, "அச்சிடு") == 0) {
            Token var_t = get_next_token(file);
            
            // பிராக்கெட் இருந்தால் தாண்டுவோம்
            if (var_t.type == 15 || strcmp(var_t.value, "(") == 0) { 
                var_t = get_next_token(file);
            }
            
            fprintf(stderr, "[Parser] Printing Variable: %s\n", var_t.value);
            tamizhi_gen_print(var_t.value);
            continue;
        }

        // 3. 'சு' (Loop)
        else if (t.type == T_FOR || strcmp(t.value, "சு") == 0) {
            while ((t = get_next_token(file)).type != T_NUM && t.type != T_EOF);
            if (t.type == T_NUM) {
                tamizhi_gen_loop_test(atoi(t.value));
            }
            while ((t = get_next_token(file)).type != 22 && t.type != T_EOF);
            skip_block(file);
            continue;
        }

        // 4. மத்த பிளாக்குகளைத் தாண்டுதல்
        else if (t.type == T_MAIN || t.type == T_FUNC || t.type == 22) {
            skip_block(file);
            continue;
        }
    }
}

void skip_block(FILE *file) {
    Token t;
    int brace_count = 1;
    while (brace_count > 0 && (t = get_next_token(file)).type != T_EOF) {
        if (t.type == 22) brace_count++; 
        if (t.type == 23) brace_count--; 
    }
}
