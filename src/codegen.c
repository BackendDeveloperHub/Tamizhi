#include "codegen.h"
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <ctype.h>

// --- DNA-VM லாஜிக் அறிவிப்புகள் ---
extern void encode_logic(const char* input_path, const char* output_path);
extern void decode_logic(const char* dna_path, const char* output_path);

// குளோபல் வேரியபிள்கள்
LLVMModuleRef module;
LLVMBuilderRef builder;
LLVMTypeRef printf_type;
LLVMValueRef printf_func;
LLVMValueRef i_ptr = NULL; 

// இஃப்-எல்ஸ்-க்குத் தேவையான குளோபல் பிளாக்குகள்
LLVMBasicBlockRef then_block, else_block, merge_block;

typedef struct {
    char name[50];
    LLVMValueRef alloca_ptr;
} Variable;

Variable symbol_table[100];
int var_count = 0;

// ஸ்டோரேஜ் பாத் கண்டுபிடிக்கும் பங்க்ஷன்
void get_tamizhi_storage_path(char* var_name, char* output_path) {
    char exe_path[1024];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path)-1);
    if (len != -1) {
        exe_path[len] = '\0';
        char* dir = dirname(exe_path);
        sprintf(output_path, "%s/storage/%s.dna", dir, var_name);
    }
}

// வேரியபிளை DNA-வாக மாற்றி ஸ்டோரேஜில் சேமிக்கும் பங்க்ஷன்
void tamizhi_dna_secure_storage(char* name, int value) {
    char temp_raw[100], dna_file[2048];
    sprintf(temp_raw, "temp_%s.txt", name);
    get_tamizhi_storage_path(name, dna_file);

    FILE *f = fopen(temp_raw, "w");
    if (f) {
        fprintf(f, "%d", value);
        fclose(f);
        encode_logic(temp_raw, dna_file);
        remove(temp_raw);
        fprintf(stderr, " [DNA-VM] '%s' தரவு பாதுகாப்பாகச் சேமிக்கப்பட்டது.\n", name);
    }
}

void tamizhi_codegen_init() {
    module = LLVMModuleCreateWithName("tamizhi_engine");
    builder = LLVMCreateBuilder();

    LLVMTypeRef printf_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
    printf_type = LLVMFunctionType(LLVMInt32Type(), printf_args, 1, 1);
    printf_func = LLVMAddFunction(module, "printf", printf_type);

    fprintf(stderr," [Codegen] LLVM + DNA Engine initialized.\n");
}

void tamizhi_generate_entry() {
    LLVMTypeRef main_func_type = LLVMFunctionType(LLVMInt32Type(), NULL, 0, 0);
    LLVMValueRef main_func = LLVMAddFunction(module, "main", main_func_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_func, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
}

void tamizhi_gen_var(char* name, int value) {
    if (var_count >= 100) return;

    tamizhi_dna_secure_storage(name, value);

    LLVMValueRef alloca = LLVMBuildAlloca(builder, LLVMInt32Type(), name);
    LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), value, 0), alloca);

    strcpy(symbol_table[var_count].name, name);
    symbol_table[var_count].alloca_ptr = alloca;
    var_count++;
}

void tamizhi_gen_print(char* var_name) {
    LLVMValueRef fmt = LLVMBuildGlobalStringPtr(builder, "%d\n", "fmt");
    LLVMValueRef val = NULL;
    char dna_file[2048];

    for(int i = 0; i < var_count; i++) {
        if(strcmp(symbol_table[i].name, var_name) == 0) {
            val = LLVMBuildLoad2(builder, LLVMInt32Type(), symbol_table[i].alloca_ptr, "load_val");
            break;
        }
    }

    if(!val && i_ptr && strcmp(var_name, "i") == 0) {
        val = LLVMBuildLoad2(builder, LLVMInt32Type(), i_ptr, "load_val");
    }

    if (!val) {
        get_tamizhi_storage_path(var_name, dna_file);
        if (access(dna_file, F_OK) == 0) { 
            decode_logic(dna_file, "temp_recovery.txt");
            FILE *res = fopen("temp_recovery.txt", "r");
            int recovered_val = 0;
            if(res) {
                if(fscanf(res, "%d", &recovered_val) == 1) {
                    val = LLVMConstInt(LLVMInt32Type(), recovered_val, 0);
                }
                fclose(res);
                remove("temp_recovery.txt"); 
            }
        }
    }

    if(val) {
        LLVMValueRef args[] = { fmt, val };
        LLVMBuildCall2(builder, printf_type, printf_func, args, 2, "print_call");
    }
}

void tamizhi_gen_var_add(char* res_name, char* var1, char* var2) {
    LLVMValueRef v1_ptr = NULL, v2_ptr = NULL;
    for(int i = 0; i < var_count; i++) {
        if(strcmp(symbol_table[i].name, var1) == 0) v1_ptr = symbol_table[i].alloca_ptr;
        if(strcmp(symbol_table[i].name, var2) == 0) v2_ptr = symbol_table[i].alloca_ptr;
    }

    if(v1_ptr && v2_ptr) {
        LLVMValueRef val1 = LLVMBuildLoad2(builder, LLVMInt32Type(), v1_ptr, "v1");
        LLVMValueRef val2 = LLVMBuildLoad2(builder, LLVMInt32Type(), v2_ptr, "v2");
        LLVMValueRef sum = LLVMBuildAdd(builder, val1, val2, "sum_tmp");

        LLVMValueRef res_ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), res_name);
        LLVMBuildStore(builder, sum, res_ptr);

        strcpy(symbol_table[var_count].name, res_name);
        symbol_table[var_count].alloca_ptr = res_ptr;
        var_count++;
    }
}

// --- நிபந்தனை லாஜிக் (Real If-Else Implementation) ---

void tamizhi_gen_if_start(char* var1, char* op, char* var2) {
    LLVMValueRef v1 = NULL, v2 = NULL;

    for(int i = 0; i < var_count; i++) {
        if(strcmp(symbol_table[i].name, var1) == 0) 
            v1 = LLVMBuildLoad2(builder, LLVMInt32Type(), symbol_table[i].alloca_ptr, "v1");
    }

    if(isdigit(var2[0])) {
        v2 = LLVMConstInt(LLVMInt32Type(), atoi(var2), 0);
    } else {
        for(int i = 0; i < var_count; i++) {
            if(strcmp(symbol_table[i].name, var2) == 0)
                v2 = LLVMBuildLoad2(builder, LLVMInt32Type(), symbol_table[i].alloca_ptr, "v2");
        }
    }

    if(!v1 || !v2) return;

    LLVMIntPredicate pred = LLVMIntEQ; 
    if(strcmp(op, "<") == 0) pred = LLVMIntSLT;
    if(strcmp(op, ">") == 0) pred = LLVMIntSGT;

    LLVMValueRef cond = LLVMBuildICmp(builder, pred, v1, v2, "if_cond");

    LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
    then_block = LLVMAppendBasicBlock(func, "then");
    else_block = LLVMAppendBasicBlock(func, "else");
    merge_block = LLVMAppendBasicBlock(func, "if_cont");

    LLVMBuildCondBr(builder, cond, then_block, else_block);
    LLVMPositionBuilderAtEnd(builder, then_block);
}

void tamizhi_gen_else_start() {
    if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder)) == NULL) {
        LLVMBuildBr(builder, merge_block);
    }
    LLVMPositionBuilderAtEnd(builder, else_block);
}

void tamizhi_gen_if_end() {
    // 1. தற்போதைய பிளாக்கில் இருந்து மெர்ஜ் பிளாக்குக்கு ஒரு கட்டாய ஜம்ப்
    if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder)) == NULL) {
        LLVMBuildBr(builder, merge_block);
    }
    
    // 2. பில்டரை மெர்ஜ் பிளாக்கிற்கு நகர்த்து
    LLVMPositionBuilderAtEnd(builder, merge_block);
}

// --- லூப் பங்க்ஷன்கள் ---

void tamizhi_gen_loop_start(int limit) {
    fprintf(stderr, " [Codegen] Loop Support Enabled for: %d cycles\n", limit);
}

void tamizhi_gen_loop_end() {
    fprintf(stderr, " [Codegen] Loop End\n");
}

void tamizhi_gen_loop_test(int limit) {
    tamizhi_gen_loop_start(limit);
}

void tamizhi_codegen_finish() {
    LLVMBuildRet(builder, LLVMConstInt(LLVMInt32Type(), 0, 0)); 
    char *ir_string = LLVMPrintModuleToString(module);
    printf("%s", ir_string); 
    LLVMDisposeMessage(ir_string);
    LLVMDisposeBuilder(builder);
}
