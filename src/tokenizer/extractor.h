/**
 * @file file_util.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief File Utils for tokenizer
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "tokenizer.h"

struct Token;

/**
 * @brief Extract a Token for a preprocessor directive.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @return Returns a Token
 */
Token extract_token_preprocessor(char* content, size_t* idx);

/**
 * @brief Extract a Token for an identifier.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @return Returns a Token
 */
Token extract_token_identifier(char* content, size_t* idx);

/**
 * @brief Extracts any literal number for expressions.
 * 
 * @param content String to extract from
 * @param idx Index pointer to increment
 * @return Token to be returned
 */
Token extract_token_literal(char* content, size_t* idx);

/**
 * @brief Extract a Token for a string literal.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @return Returns a Token
 */
Token extract_token_string(char* content, size_t* idx);