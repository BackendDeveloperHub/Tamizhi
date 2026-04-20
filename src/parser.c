#include "parser.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// கணிதக் கணக்கீடுகளை கையாள (e.g., 1 + 4)
void parse_expression(FILE *file, Token first_token) {
    Token op = get_next_token(file); 
    if (strcmp(op.value, "+") == 0) {
        Token second_val = get_next_token(file);
        fprintf(stderr, "[Parser] கணிதம்: %s + %s\n", first_token.value, second_val.value);

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
            fprintf(stderr, "[Parser] முதன்மை பகுதி கண்டறியப்பட்டது.\n");
            // '{' வரும் வரை டோக்கன்களை நகர்த்தவும்
            while ((t = get_next_token(file)).type != 22 && t.type != T_EOF); 
            // '}' வரும் வரை உள்ளே இருப்பவற்றை ஸ்கிப் செய்யவும்
            while ((t = get_next_token(file)).type != 23 && t.type != T_EOF);
        }

        // --- 'நிகழ்' (Function Definition) ---
        else if (t.type == T_FUNC) {
            Token func_name = get_next_token(file);
            fprintf(stderr, "[Parser] செயல்முறை உருவாக்கம்: %s\n", func_name.value);
            // அடுத்த செமிகோலன் அல்லது கோலன் வரை நகர்த்தவும்
            while ((t = get_next_token(file)).type != 17 && t.type != T_EOF);
        }

        // --- 'இயக்கு' (Function Call / Execute) ---
        else if (t.type == T_CALL) {
            fprintf(stderr, "[Parser] 'இயக்கு' பகுதி தொடங்குகிறது...\n");
            
            // 'add' அல்லது ஃபங்க்ஷன் பெயர் வரும் வரை தேடவும்
            while ((t = get_next_token(file)).type != T_ID && t.type != T_EOF);
            char func_name[64];
            strcpy(func_name, t.value);

            // '(' வரும் வரை நகர்த்தவும்
            while ((t = get_next_token(file)).type != 15 && t.type != T_EOF);

            // முதல் எண்ணை எடுக்கவும் (உதாரணம்: 1)
            Token first_num = get_next_token(file);
            if (first_num.type == T_NUM) {
                parse_expression(file, first_num);
            }

            fprintf(stderr, "[Parser] செயல்முறை '%s' வெற்றிகரமாகத் தொகுக்கப்பட்டது.\n", func_name);
        }
    }
}
