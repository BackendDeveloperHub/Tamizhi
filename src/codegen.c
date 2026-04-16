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

// Variable declaration logic
void tamizhi_gen_var_decl(char* name, int initial_value) {
    LLVMValueRef alloca = LLVMBuildAlloca(builder, LLVMInt32Type(), name);
    LLVMValueRef val = LLVMConstInt(LLVMInt32Type(), initial_value, 0);
    LLVMBuildStore(builder, val, alloca);
    printf("[Codegen] Variable '%s' created.\n", name);
}

// 1 Million Loop Generation Logic
void tamizhi_gen_loop_test(int limit) {
    LLVMValueRef main_func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));

    // 1. Blocks create pannuvom
    LLVMBasicBlockRef cond_block = LLVMAppendBasicBlock(main_func, "loop_cond");
    LLVMBasicBlockRef body_block = LLVMAppendBasicBlock(main_func, "loop_body");
    LLVMBasicBlockRef after_block = LLVMAppendBasicBlock(main_func, "loop_after");

    // 2. Loop counter 'i' initialize
    LLVMValueRef i_ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "i");
    LLVMBuildStore(builder, LLVMConstInt(LLVMInt32Type(), 0, 0), i_ptr);

    LLVMBuildBr(builder, cond_block);
    LLVMPositionBuilderAtEnd(builder, cond_block);

    // 3. Condition: i < limit
    LLVMValueRef i_val = LLVMBuildLoad2(builder, LLVMInt32Type(), i_ptr, "i_val");
    LLVMValueRef test_limit = LLVMConstInt(LLVMInt32Type(), limit, 0);
    LLVMValueRef cond = LLVMBuildICmp(builder, LLVMIntSLT, i_val, test_limit, "tmp_cond");
    LLVMBuildCondBr(builder, cond, body_block, after_block);

    // 4. Body: i = i + 1
    LLVMPositionBuilderAtEnd(builder, body_block);
    LLVMValueRef next_val = LLVMBuildAdd(builder, i_val, LLVMConstInt(LLVMInt32Type(), 1, 0), "next_i");
    LLVMBuildStore(builder, next_val, i_ptr);
    LLVMBuildBr(builder, cond_block);

    LLVMPositionBuilderAtEnd(builder, after_block);
    printf("[Codegen] 1 Million Loop logic generated.\n");
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
