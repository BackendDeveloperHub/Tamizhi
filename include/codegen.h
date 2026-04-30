#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>

// 1. கம்பைலர் தொடக்க மற்றும் முடிவு நிலைகள்
void tamizhi_codegen_init();
void tamizhi_generate_entry();
void tamizhi_codegen_finish();

// 2. வேரியபிள் மேனேஜ்மென்ட்
void tamizhi_gen_var(char* name, int initial_value);
void tamizhi_gen_var_add(char* res_name, char* var1, char* var2);

// 3. அச்சிடுதல் (Printing)
void tamizhi_gen_print(char* var_name);
void tamizhi_gen_add_and_print(int n1, int n2);

// 4. லூப் லாஜிக் (Looping)
void tamizhi_gen_loop_test(int limit); // பழைய பங்க்ஷன்
void tamizhi_gen_loop_start(int limit); // புதிய லூப் தொடக்கம்
void tamizhi_gen_loop_end();           // புதிய லூப் முடிவு

// 5. நிபந்தனை லாஜிக் (If-Else Logic)
void tamizhi_gen_if_start(char* var1, char* op, char* var2);
void tamizhi_gen_else_start();
void tamizhi_gen_if_end();

#endif
