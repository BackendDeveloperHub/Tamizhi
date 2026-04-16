#include "codegen.h"
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <stdio.h>

// LLVM Global variables
LLVMModuleRef module;
LLVMBuilderRef builder;

void tamizhi_codegen_init() {
    // 1. Module create pannuvom
    module = LLVMModuleCreateWithName("tamizhi_engine");

    // 2. Builder create pannuvom
    builder = LLVMCreateBuilder();

    printf("[Codegen] LLVM Engine initialized successfully on your phone!\n");
}

void tamizhi_generate_entry() {
    // 3. 'main' function-a create pannanum
    LLVMTypeRef return_type = LLVMInt32Type();
    LLVMTypeRef main_func_type = LLVMFunctionType(return_type, NULL, 0, 0);
    LLVMValueRef main_func = LLVMAddFunction(module, "main", main_func_type);

    // 4. Function kulla enter aaguvom (Entry block)
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_func, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);

    printf("[Codegen] Main function entry point created.\n");
}

void tamizhi_codegen_finish() {
    // 5. TERMINATOR: Function-a mudikka 'return 0' add pannuvom
    // Ithu illama thaan pona vaati error vandhuchi
    LLVMValueRef ret_val = LLVMConstInt(LLVMInt32Type(), 0, 0);
    LLVMBuildRet(builder, ret_val); 

    // 6. Code-a verify pannanum
    char *error = NULL;
    LLVMVerifyModule(module, LLVMAbortProcessAction, &error);

    // 7. Inga thaan unga Native IR screen-la print aagum
    printf("\n--- Generated LLVM IR ---\n");
    LLVMDumpModule(module);

    // Clean up
    LLVMDisposeBuilder(builder);
}
