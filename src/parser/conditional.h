#pragma once
#include "parser.h"

/**
 * @brief Different types of conditionals (IF, ELSE IF, ELSE)
 */
typedef enum{
    CONDITIONAL_TYPE_IF,
    CONDITIONAL_TYPE_ELSE_IF,
    CONDITIONAL_TYPE_ELSE,
}ConditionalType;

/**
 * @brief Conditional
 * 
 */
typedef struct{
    ConditionalType type;
    void* condition;

    struct ScopeBlock* conditional_content;
} Conditional;

/**
 * @brief Make conditional if
 * 
 * @param token_list Token list
 * @param idx Index of current token
 * @param statement_list List of statements
 * @param block Block for said statement list
 * @param else_if Else If Bool?
 */
void make_conditional_if(List* token_list, size_t* idx, List* statement_list, struct ScopeBlock* block, int else_if);

/**
 * @brief Make conditional else
 * 
 * @param token_list Token list
 * @param idx Index of current token
 * @param statement_list List of statements
 * @param block Block for said statement list
 */
void make_conditional_else(List* token_list, size_t* idx, List* statement_list, struct ScopeBlock* block);