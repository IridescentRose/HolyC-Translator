#pragma once
#include "parser.h"

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
 * @brief Makes a general-type expression
 * 
 * @param token_list Token list to look at
 * @param identifier Identifier of first token
 * @param idx Index
 * @param statement_list List to add to
 */
void make_expression_general(List* token_list, StringSlice identifier, size_t* idx, List* statement_list);

/**
 * @brief Makes a general-type expression from compound assignment
 * 
 * @param token_list Token list to look at
 * @param identifier Identifier of first token
 * @param idx Index
 * @param statement_list List to add to
 */
void make_expression_compound_assign(List* token_list, StringSlice identifier, size_t* idx, List* statement_list);

/**
 * @brief Make a printf expression based on string literal
 * 
 * @param token_list Token list
 * @param token Token first picked
 * @param idx Index of token
 * @param statement_list Statement list to add to
 */
void make_expression_printf(List* token_list, Token* token, size_t* idx, List* statement_list);
