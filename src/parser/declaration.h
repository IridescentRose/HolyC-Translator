#pragma once
#include "expression.h"
#include "parser.h"

/**
 * @brief Declaration object which contains the type and identifier, if it is a point or a function, if it externally linked, and arguments if it is a function
 * 
 */
typedef struct{
    StringSlice identifier;
    CType type;

    char is_function;
    Arguments args;

    void* expr;
} Declaration;

/**
 * @brief Makes a variable declaration statement
 * 
 * @param list List of statements to add to
 * @param identifier Slice of identifier
 * @param type Type of identifier
 */
void make_variable_declaration(List* list, StringSlice identifier, CType type);

/**
 * @brief Makes a variable declaration statement
 * 
 * @param token_list List of tokens
 * @param idx Index to read from
 * @param identifier Identifier Slice
 * @param type Type of Identifier
 * @param statement_list List of statements to add to
 */
void make_variable_declaration_assign(List* token_list, size_t* idx, StringSlice identifier, CType type, List* statement_list);

/**
 * @brief Makes a variable declaration statement
 * 
 * @param list List of statements to add to
 * @param identifier Slice of identifier
 * @param type Type of identifier
 * @param args If the declaration is a function - what are the arguments?
 */
void make_function_declaration(List* list, StringSlice identifier, CType type, Arguments args);
