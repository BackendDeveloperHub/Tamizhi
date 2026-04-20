#include "parser.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// டோக்கன் நாம் எதிர்பார்ப்பது தானா என்று சரிபார்க்கும் உதவி ஃபங்க்ஷன்
void expect(int expected_type, FILE *file) {
    Token t = get_next_token(file);
    if ((int)t.type != expected_type) {
        fprintf(stderr, "[Error] வரிசையில் பிழை! எதிர்பார்த்தது %d, ஆனால் கிடைத்தது %s\n", expected_type, t.value);
    }
}

// 1. கணிதக் கணக்கீடுகளை கையாள (e.g., 1 + 4)
void parse_expression(FILE *file, Token first_token) {
    Token op = get_next_token(file); // '+'
    Token second_val = get_next_token(file); // '4'
    
    if (strcmp(op.value, "+") == 0) {
        fprintf(stderr, "[Parser] கணிதம் கண்டறியப்பட்டது: %s + %s\n", first_token.value, second_val.value);
        
        // Backend-க்கு தகவல் அனுப்பி கூட்டல் கோடை உருவாக்குதல்
        int n1 = atoi(first_token.value);
        int n2 = atoi(second_val.value);
        tamizhi_gen_add_and_print(n1, n2); 
    }
}

void parse(FILE *file) {
    Token t;
    while ((t = get_next_token(file)).type != T_EOF) {

        // --- 'முதன்மை' (Main Block) ---
        if (t.type == T_MAIN) {
            expect(15, file); // '('
            expect(16, file); // ')'
            expect(22, file); // '{'
            fprintf(stderr, "[Parser] முதன்மை பகுதிக்குள் நுழைகிறது...\n");
            
            // உள்ளே இருக்கும் '@add' போன்றவற்றை இப்போதைக்கு ஸ்கிப் செய்கிறோம்
            Token inner = get_next_token(file);
            while (strcmp(inner.value, "}") != 0) {
                inner = get_next_token(file);
            }
            expect(17, file); // ';'
        }

        // --- 'நிகழ்' (Function Definition) ---
        else if (t.type == T_FUNC) {
            Token func_name = get_next_token(file); // 'add'
            expect(15, file); // '('
            expect(16, file); // ')'
            expect(17, file); // ':' (உங்க இமேஜில் இருந்தது)
            fprintf(stderr, "[Parser] செயல்முறை (Function) உருவாக்கம்: %s\n", func_name.value);
        }

        // --- 'அச்சிடு' (Print) ---
        else if (t.type == T_PRINT) {
            expect(15, file); // '('
            Token var_name = get_next_token(file); // 'a' அல்லது 'b'
            // இப்போதைக்கு எக்ஸ்பிரஷனை மட்டும் ஹேண்டில் செய்கிறோம்
            expect(16, file); // ')'
            expect(17, file); // ';'
        }

        // --- 'இயக்கு' (Function Call / Execute) ---
        else if (t.type == T_CALL) {
            expect(15, file); // '('
            expect(22, file); // '{'
            
            Token func_to_run = get_next_token(file); // 'add'
            expect(15, file); // '('
            
            Token first_num = get_next_token(file); // '1'
            parse_expression(file, first_num);     // '1 + 4'-ஐக் கையாளும்
            
            expect(16, file); // ')'
            expect(17, file); // ';'
            expect(23, file); // '}'
            expect(17, file); // ';'
            fprintf(stderr, "[Parser] செயல்முறை '%s' இயக்கப்படுகிறது.\n", func_to_run.value);
        }
    }
}
