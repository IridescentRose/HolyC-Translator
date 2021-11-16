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
    TOKEN_TYPE_TERMINATOR,  /* Literally just semicolon */
    TOKEN_TYPE_IDENTIFIER,  /* Any valid identifier - this includes function names and primitives. */
    TOKEN_TYPE_PUNCTUATOR,  /* Parentheses () OR Brackets [] */
    TOKEN_TYPE_SCOPING,     /* Define a scope using braces {} */
    TOKEN_TYPE_ARITHMETIC,  /* Arithmetic operators (+ - * / % ++ --) */
    TOKEN_TYPE_BITWISE,     /* Bitwise operators (& | ^ ~ << >>) */
    TOKEN_TYPE_LOGIC,       /* Logical operators (&& || !) */
    TOKEN_TYPE_RELATION,    /* Relational operators (== != > < <= >= <=) */
    TOKEN_TYPE_ASSIGNMENT,  /* Assignment operators (Arithmetic/Bitwise + = or solo =) */
    TOKEN_TYPE_MISC,        /* Miscellaneous operators (sizeof, ?) */
    TOKEN_TYPE_PREPROCESSOR,/* Starts with a # */
    TOKEN_TYPE_IMMEDIATE,   /* Starts with a number [0-9] */
    TOKEN_TYPE_STRING,      /* Starts with "" */
    TOKEN_TYPE_NEWLINE,     /* '\n' I'm not sure if I need this - I just want to keep track. */
} TokenType;

/**
 * @brief A sliced string - ptr to unique memory address, length is the length of the 0 terminated slice
 * 
 */
typedef struct{
    char* ptr;
    size_t len;
} StringSlice;

typedef struct{
    TokenType type;
    StringSlice slice;
} Token;

void tokenize(const char* filename);