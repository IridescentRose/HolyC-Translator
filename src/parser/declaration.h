#pragma once
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
 * @param list List of statements to add to
 * @param identifier Slice of identifier
 * @param type Type of identifier
 * @param args If the declaration is a function - what are the arguments?
 */
void make_function_declaration(List* list, StringSlice identifier, CType type, Arguments args);
