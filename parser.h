/**
 * @file parser.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Parser for HolyC
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "tokenizer.h"

/**
 * @brief Statements are comprised of 3 types: Declaration, Definition, and Expressions. 
 * Declaration means a variable or function is declared. 
 * Definition means a variable or function is defined in the same statement, declarations may be implied from a definition.
 * Expressions are statements that reflect changes in code using only existing variables and/or immediates or literals.
 *
 */
typedef enum{
    STATEMENT_TYPE_PREPROCESSOR,
    STATEMENT_TYPE_DECLARATION,
    STATEMENT_TYPE_DEFINITION,
    STATEMENT_TYPE_EXPRESSION,
}StatementType;

/**
 * @brief Statements contain a type and corresponding type data. The data is laid out in other structs.
 *
 */
typedef struct{
    StatementType type;
    void* statementData;
} Statement;

typedef enum{
    TYPE_U0,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_F64,
} Type;


/**
 * @brief A scoped block contains a list of statements. These statements are in order and can have definitions.
 * Scope checking should be applied to every scope block and objects in scopes should be checked to exist in this
 * scope or higher order scopes.
 */
struct ScopeBlock{
    List* statement_list;
    struct ScopeBlock* parent; //May be NULL
};

typedef struct{
    Type type;
    void* data;
} Expression;

typedef struct{
    Type types[16];
    char* identifiers[16];
} Arguments;

typedef struct{
    StringSlice identifier;
    Type type;
    char pointer;
    char is_function;
    Arguments args;
} Declaration;

typedef struct{
    StringSlice identifier;
    Type type;
    char pointer;
    char is_function;
    Arguments args;

    //Function definition
    struct ScopeBlock* function_content;
} Definition;

typedef struct{
    StringSlice text;
} PreProcessor;

/**
 * @brief A program is just a big globally scoped block for our purposes
 *
 */
typedef struct{
    struct ScopeBlock block;
}Program;

void free_program(struct ScopeBlock* program);
Program* parse(List* token_list);