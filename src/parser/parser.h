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
#include "../tokenizer/tokenizer.h"
#include "utility_types.h"

#include "declaration.h"
#include "preprocessor.h"

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

/**
 * @brief A scoped block contains a list of statements. These statements are in order and can have definitions.
 * Scope checking should be applied to every scope block and objects in scopes should be checked to exist in this
 * scope or higher order scopes.
 */
struct ScopeBlock{
    List* statement_list;
    struct ScopeBlock* parent; //May be NULL
};

/**
 * @brief Different types of expressions
 * CALL expressions generate a default function call from implied text.
 * PRINTF expressions generate PRINTF calls from implied text.
 * GENERAL expressions are any type of expression.
 */
typedef enum{
    EXPRESSION_TYPE_GENERAL,
    EXPRESSION_TYPE_CALL,
    EXPRESSION_TYPE_PRINTF,
}ExpressionType;

/**
 * @brief Expression object, comprised of a type, and a buffer to store the expression text
 * 
 */
typedef struct{
    ExpressionType type;
    char buffer[256];
} Expression;



/**
 * @brief Definition object which contains the type and identifier, if it is a point or a function, if it externally linked, and arguments , also contains a codeblock
 * 
 */
typedef struct{
    StringSlice identifier;
    Type type;
    char pointer;
    char is_function;
    Arguments args;

    //Function definition
    struct ScopeBlock* function_content;
} Definition;

/**
 * @brief A program is just a big globally scoped block for our purposes
 *
 */
typedef struct{
    struct ScopeBlock block;
}Program;

Program* parse(List* token_list);


/**
 * @brief Frees a given Scoped Block
 * 
 * @param program Scope Block to free
 */
void free_program(struct ScopeBlock* program);
