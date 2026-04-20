#ifndef CODEGEN_H
#define CODEGEN_H

#include <llvm-c/Core.h>

// கம்பைலர் தொடக்க நிலைகள்
void tamizhi_codegen_init();
void tamizhi_generate_entry();
void tamizhi_codegen_finish();

// வேரியபிள் மற்றும் லூப் லாஜிக்
void tamizhi_gen_var(char* name, int initial_value);
void tamizhi_gen_loop_test(int limit);

// --- புதிய பங்க்ஷன்கள்: வேரியபிளை பிரிண்ட் செய்ய மற்றும் கூட்டல் செய்ய ---
void tamizhi_gen_print(char* var_name);         // இதோ இந்த வரிதான் மிஸ்ஸிங்!
void tamizhi_gen_add_and_print(int n1, int n2); 

#endif
