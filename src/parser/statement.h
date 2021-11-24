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
 * @brief Makes a function definition
 * 
 * @param list List to add statement to
 * @param identifier Name
 * @param type Type
 * @param pointer Is a pointer?
 * @param args Arguments list
 * @param statement_list Statements inside the block
 */
void make_function_definition(List* list, StringSlice identifier, Type type, char pointer, Arguments args, struct ScopeBlock* statement_list);
