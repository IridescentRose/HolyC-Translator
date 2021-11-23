/**
 * @file extractor.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Token extractor
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
 * @brief Extract a Token for a single character
 * 
 * @param content String to extract from
 * @param idx Index pointer to increment
 * @param line Line number
 * @param cursor Cursor number
 * @param token_list Token list
 * @return Token 
 */
void extract_token_single_char(char* content, size_t* idx, TokenType type, int line, int* cursor, List* token_list);

/**
 * @brief Extract a Token for a preprocessor directive.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @param line Line number
 * @param cursor Cursor number
 * @param token_list Token list
 * @return Returns a Token
 */
void extract_token_preprocessor(char* content, size_t* idx, int line, int* cursor, List* token_list);

/**
 * @brief Extract a Token for an identifier.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @param line Line number
 * @param cursor Cursor number
 * @return Returns a Token
 */
Token extract_token_identifier(char* content, size_t* idx, int line, int* cursor);

/**
 * @brief Extracts any literal number for expressions.
 * 
 * @param content String to extract from
 * @param idx Index pointer to increment
 * @param line Line number
 * @param cursor Cursor number
 * @return Token to be returned
 */
Token extract_token_literal(char* content, size_t* idx, int line, int* cursor);

/**
 * @brief Extract a Token for a string literal.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @param line Line number
 * @param cursor Cursor number
 * @param token_list Token list
 * @return Returns a Token
 */
void extract_token_string(char* content, size_t* idx, int line, int* cursor, List* token_list);
