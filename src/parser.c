#include "parser.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 1. Prototype
void skip_block(FILE *file); 

void parse(FILE *file) {
    Token t;
    fprintf(stderr, "[Parser] Tamizhi Engine Analysis Started...\n");

    while ((t = get_next_token(file)).type != T_EOF) {

        // --- 'முதன்மை' (Main Block) ---
        if (t.type == T_MAIN) {
            fprintf(stderr, "[Parser] Main block detected.\n");
            while ((t = get_next_token(file)).type != 22 && t.type != T_EOF); 
            skip_block(file);
            continue;
        }

        // --- 'நிகழ்' (Function Definition) ---
        else if (t.type == T_FUNC) {
            fprintf(stderr, "[Parser] Function definition detected.\n");
            while ((t = get_next_token(file)).type != 17 && t.type != 22 && t.type != T_EOF);
            if (t.type == 22) skip_block(file);
            continue;
        }

        // --- 'சு' (Loop - ஸ்பீடு டெஸ்ட் செய்ய) ---
        else if (t.type == T_FOR || strcmp(t.value, "சு") == 0) {
            fprintf(stderr, "[Parser] Loop detection started...\n");

            // லூப் லிமிட் (எண்) கிடைக்கும் வரை தேடுவோம்
            while ((t = get_next_token(file)).type != T_NUM && t.type != T_EOF);
            
            if (t.type == T_NUM) {
                int limit = atoi(t.value);
                fprintf(stderr, "[Parser] Loop limit set to: %d\n", limit);
                
                // codegen.c-ல் இருக்கும் LLVM Loop லாஜிக்கை இங்க கூப்பிடுறோம்
                tamizhi_gen_loop_test(limit);
            }
            
            // லூப் பாடியைத் தாண்ட (தற்போதைக்கு ஸ்கிப் செய்வோம்)
            while ((t = get_next_token(file)).type != 22 && t.type != T_EOF);
            skip_block(file);
            continue;
        }

        // --- 'இயக்கு' (Execution Block) ---
        else if (t.type == T_CALL || strcmp(t.value, "இயக்கு") == 0) {
            fprintf(stderr, "[Parser] Starting Execution (இயக்கு)...\n");

            while (1) {
                t = get_next_token(file);
                if (t.type == 23 || t.type == T_EOF) break; 

                if (t.value[0] >= '0' && t.value[0] <= '9') {
                    int n1 = atoi(t.value);
                    fprintf(stderr, "[Parser] Found First Number: %d\n", n1);

                    while (1) {
                        t = get_next_token(file);
                        if (t.type == 23 || t.type == T_EOF) break;
                        if (t.value[0] >= '0' && t.value[0] <= '9') break;
                    }

                    if (t.value[0] >= '0' && t.value[0] <= '9') {
                        int n2 = atoi(t.value);
                        fprintf(stderr, "[Parser] Found Second Number: %d\n", n2);
                        fprintf(stderr, "[Parser] Logic Found: %d + %d\n", n1, n2);
                        
                        tamizhi_gen_add_and_print(n1, n2);
                        break; 
                    }
                }
            }
        }
    }
}

// 2. skip_block - பிளாக்குகளைத் தாண்ட
void skip_block(FILE *file) {
    Token t;
    int brace_count = 1;
    while (brace_count > 0 && (t = get_next_token(file)).type != T_EOF) {
        if (t.type == 22) brace_count++; // '{'
        if (t.type == 23) brace_count--; // '}'
    }
}
