#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "lexer.h"

// AST Node vagaigal (Types of nodes in our tree)
typedef enum {
    NODE_VAR_DECL,   // Variable declaration (e.g., எண் i = 0)
    NODE_PRINT,      // Output (e.g., கூறு)
    NODE_BINARY_OP,  // Math operations (+, -, *, /)
    NODE_LOOP        // Loop logic (e.g., சு)
} NodeType;

// Abstract Syntax Tree Structure
typedef struct ASTNode {
    NodeType type;          // Node-oda vagai
    Token token;            // Intha node-kulla irukira token (value/name)
    struct ASTNode *left;   // Idathu pakkam branch
    struct ASTNode *right;  // Valathu pakkam branch
} ASTNode;

// Function declarations
void parse(FILE *file);                      // Parser-a start panna
ASTNode* create_node(NodeType type, Token t); // Puthu node create panna

#endif
