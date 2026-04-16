#include "codegen.h"
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

// LLVM Global variables
LLVMModuleRef module;
LLVMBuilderRef builder;

void tamizhi_codegen_init() {
    module = LLVMModuleCreateWithName("tamizhi_engine");
    builder = LLVMCreateBuilder();
    printf("[Codegen] LLVM Engine initialized successfully on your phone!\n");
}

void tamizhi_generate_entry() {
    LLVMTypeRef return_type = LLVMInt32Type();
    LLVMTypeRef main_func_type = LLVMFunctionType(return_type, NULL, 0, 0);
    LLVMValueRef main_func = LLVMAddFunction(module, "main", main_func_type);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_func, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    printf("[Codegen] Main function entry point created.\n");
}

// Variable create panna intha function-a use pannuvom
void tamizhi_gen_var_decl(char* name, int initial_value) {
    LLVMValueRef alloca = LLVMBuildAlloca(builder, LLVMInt32Type(), name);
    LLVMValueRef val = LLVMConstInt(LLVMInt32Type(), initial_value, 0);
    LLVMBuildStore(builder, val, alloca);
    printf("[Codegen] Variable '%s' created with value %d\n", name, initial_value);
}

void tamizhi_codegen_finish() {
    LLVMValueRef ret_val = LLVMConstInt(LLVMInt32Type(), 0, 0);
    LLVMBuildRet(builder, ret_val); 

    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);

    printf("\n--- Generated LLVM IR ---\n");
    LLVMDumpModule(module);

    LLVMDisposeBuilder(builder);
}
