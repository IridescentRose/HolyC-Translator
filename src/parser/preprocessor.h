/**
 * @file preprocessor.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Contains methods related to preprocessor statements
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "parser.h"

/**
 * @brief Makes a preprocessor statement
 * 
 * @param list List of statements to add to
 * @param token Token of the preprocessor statement
 */
void make_preprocessor(List* list, Token* token, size_t* idx);
