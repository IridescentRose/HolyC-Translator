/**
 * @file util.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief C utilities
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "util.h"

char* dupe_string(const char* str1) {
    /* TIL: Null Terminator is not included in strlen() and valgrind throws a fit. */
    char* res = calloc(strlen(str1) + 1, sizeof(char));

    return strcpy(res, str1);
}