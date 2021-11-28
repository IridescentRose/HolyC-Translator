/**
 * @file statement.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Creates statements
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "parser.h"


/**
 * @brief Definition object which contains the type and identifier, if it is a point or a function, if it externally linked, and arguments , also contains a codeblock
 * 
 */
typedef struct{
    StringSlice identifier;
    CType type;

    char is_function;
    Arguments args;

    struct ScopeBlock* function_content;
} Definition;


/**
 * @brief Makes a function definition
 * 
 * @param list List to add statement to
 * @param identifier Name
 * @param type Type
 * @param args Arguments list
 * @param statement_list Statements inside the block
 */
void make_function_definition(List* list, StringSlice identifier, CType type, Arguments args, struct ScopeBlock* statement_list);
