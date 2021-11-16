/**
 * @file util.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief C utilities
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief This macro contains the behavior for a failed check. It simply outputs an error message and surrounding data from the variadic argument to console and exits with code failure.
 */
#define CHECK_FAILED(...) fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE);

/**
 * @brief This macro performs a check to make sure x is greater than zero.
 * @param x Variable to be checked
 */
#define CHECK_GREATER_ZERO(x, ...) if( (x) <= 0 ) { CHECK_FAILED(__VA_ARGS__) }

/**
 * @brief This function is practically identical to strdup() but guaranteed to exist on all systems. The method copies a string and allocates a new memory location for said string.
 * 
 * @param str1 String to copy
 * @return char* new malloc'd string
 */
char* dupe_string(const char* str1);