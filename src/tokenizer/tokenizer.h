/**
 * @file tokenizer.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Basic tokenizer for HolyC
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "../util.h"
#include "file_util.h"

typedef enum{
    TOKEN_TYPE_TERMINATOR   = 0,    /* ; */
    TOKEN_TYPE_PUNCTUATOR   = 1,    /* ( ) , . [ ] */
    TOKEN_TYPE_SCOPING      = 2,    /* { } */
    TOKEN_TYPE_ARITHMETIC   = 3,    /* + - * / % ++ -- */
    TOKEN_TYPE_BITWISE      = 4,    /* & | ^ ~ << >> */
    TOKEN_TYPE_LOGIC        = 5,    /* && || ! */
    TOKEN_TYPE_RELATION     = 6,    /* == != > < <= >= <= */
    TOKEN_TYPE_ASSIGNMENT   = 7,    /* = (and arithmetic / bitwise assign) */
    TOKEN_TYPE_MISC         = 8,    /* ? : */
    TOKEN_TYPE_PREPROCESSOR = 9,    /* Starts with a # */
    TOKEN_TYPE_IMMEDIATE    = 10,   /* Starts with a number [0-9] */
    TOKEN_TYPE_STRING       = 11,   /* Starts with "" */
    TOKEN_TYPE_PRIMITIVE    = 12,   /* Primitives. */
    TOKEN_TYPE_KEYWORD      = 13,   /* Keywords */
    TOKEN_TYPE_IDENTIFIER   = 14    /* Identifier string */
} TokenType;

typedef enum{
    KEYWORD_TYPE_INVALID   = 0,
    KEYWORD_TYPE_SWITCH    = 1,
    KEYWORD_TYPE_CASE      = 2,
    KEYWORD_TYPE_DEFAULT   = 3,
    KEYWORD_TYPE_BREAK     = 4,
    KEYWORD_TYPE_GOTO      = 5,
    KEYWORD_TYPE_CONST     = 6,
    KEYWORD_TYPE_STATIC    = 7,
    KEYWORD_TYPE_VOLATILE  = 8,
    KEYWORD_TYPE_REGISTER  = 9,
    KEYWORD_TYPE_IF        = 10,
    KEYWORD_TYPE_ELSE      = 11,
    KEYWORD_TYPE_ENUM      = 12,
    KEYWORD_TYPE_CLASS     = 13,
    KEYWORD_TYPE_UNION     = 14,
    KEYWORD_TYPE_EXTERN    = 15,
    KEYWORD_TYPE_RETURN    = 16,
    KEYWORD_TYPE_DO        = 17,
    KEYWORD_TYPE_WHILE     = 18,
    KEYWORD_TYPE_FOR       = 19,
    KEYWORD_TYPE_SIZEOF    = 20,
} KeywordType;

/**
 * @brief A sliced string - ptr to unique memory address, length is the length of the 0 terminated slice
 * 
 */
typedef struct{
    char* ptr;
    size_t len;
} StringSlice;

/**
 * @brief A Token with a type and a string slice
 *
 */
typedef struct{
    TokenType type;
    KeywordType keyword;
    StringSlice slice;
    int line;
    int cursor;
} Token;



/**
 * @brief Tokenizes a file into a list of tokens
 * @param filename File to tokenize
 */
List* tokenize(const char* filename);

/**
 * @brief A function to free tokens from a given list.
 * @param token_list A list of tokens.
 */
void free_tokens(List* token_list);