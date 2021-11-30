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

#include "expression.h"
#include "declaration.h"
#include "preprocessor.h"
#include "definition.h"
#include "conditional.h"

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
    STATEMENT_TYPE_CONDITIONAL,
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
 * @brief A program is just a big globally scoped block for our purposes
 *
 */
typedef struct{
    struct ScopeBlock block;
}Program;

/**
 * @brief Get the next object
 * 
 * @param token_list Token List
 * @param idx Index to increments
 * @return Token* Next token
 */
Token* get_next(List* token_list, size_t* idx);

/**
 * @brief Parses a given token list into a program representation
 * 
 * @param token_list List of Tokens
 * @return Program* Program out
 */
Program* parse(List* token_list);


/**
 * @brief Frees a given Scoped Block
 * 
 * @param program Scope Block to free
 */
void free_program(struct ScopeBlock* program);

/**
 * @brief Parse tokens into a program scope block
 * 
 * This is an extremely complicated method.
 * 
 * @param block Block to process into
 * @param token_list List of all tokens
 * @param idx Index of current token 
 */
void parse_token_program(struct ScopeBlock *block, List *token_list, size_t *idx);