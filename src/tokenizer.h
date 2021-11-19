//BY SUBMITTING THIS FILE TO CARMEN, I CERTIFY THAT I HAVE STRICTLY ADHERED
//TO THE TENURES OF THE OHIO STATE UNIVERSITY’S ACADEMIC INTEGRITY POLICY
//WITH RESPECT TO THIS ASSIGNMENT.

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
#include "util.h"

typedef enum{
    TOKEN_TYPE_TERMINATOR   = 0,    /* Literally just semicolon */
    TOKEN_TYPE_IDENTIFIER   = 1,    /* Any valid identifier - this includes function names and primitives. */
    TOKEN_TYPE_PUNCTUATOR   = 2,    /* Parentheses () OR Brackets [] */
    TOKEN_TYPE_SCOPING      = 3,    /* Define a scope using braces {} */
    TOKEN_TYPE_ARITHMETIC   = 4,    /* Arithmetic operators (+ - * / % ++ --) */
    TOKEN_TYPE_BITWISE      = 5,    /* Bitwise operators (& | ^ ~ << >>) */
    TOKEN_TYPE_LOGIC        = 6,    /* Logical operators (&& || !) */
    TOKEN_TYPE_RELATION     = 7,    /* Relational operators (== != > < <= >= <=) */
    TOKEN_TYPE_ASSIGNMENT   = 8,    /* Assignment operators (Arithmetic/Bitwise + = or solo =) */
    TOKEN_TYPE_MISC         = 9,    /* Miscellaneous operators (sizeof, ?) */
    TOKEN_TYPE_PREPROCESSOR = 10,   /* Starts with a # */
    TOKEN_TYPE_IMMEDIATE    = 11,   /* Starts with a number [0-9] */
    TOKEN_TYPE_STRING       = 12,   /* Starts with "" */
    TOKEN_TYPE_PRIMITIVE    = 13,   /* Primitives. */
    TOKEN_TYPE_RETURN       = 14,   /* Return. */
    TOKEN_TYPE_EXTERN       = 15,   /* Extern. */
} TokenType;

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
    StringSlice slice;
} Token;

/**
 * @brief A function to free tokens from a given list.
 * @param token_list A list of tokens.
 */
void free_tokens(List* token_list);

/**
 * @brief Tokenizes a file into a list of tokens
 * @param filename File to tokenize
 */
List* tokenize(const char* filename);