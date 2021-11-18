//BY SUBMITTING THIS FILE TO CARMEN, I CERTIFY THAT I HAVE STRICTLY ADHERED
//TO THE TENURES OF THE OHIO STATE UNIVERSITY’S ACADEMIC INTEGRITY POLICY
//WITH RESPECT TO THIS ASSIGNMENT.

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
#define _XOPEN_SOURCE
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
 * @brief This macro performs a check to make sure x is not null.
 * @param x Variable to be checked
 */
#define CHECK_NOT_NULL(x, ...) if((x) == NULL) { CHECK_FAILED(__VA_ARGS__) }

/**
 * @brief This function is practically identical to strdup() but guaranteed to exist on all systems. The method copies a string and allocates a new memory location for said string.
 * 
 * @param str1 String to copy
 * @return char* new malloc'd string
 */
char* dupe_string(const char* str1);

/**
 * @brief Slices a string from i1 to i2 in the given str and adds null termination to the final result
 * 
 * @param str Intial string
 * @param idx Start Index
 * @param len Length of slice
 * @return char* New slice
 */
char* make_slice(const char* str, size_t idx, size_t len);

/**
 * @brief Struct for a dynamically resized Array called a List
 * 
 */
typedef struct{
    void* data; /* Generic pointer to data array. */
    size_t type_size; /* Size in bytes of the type of the array*/

    size_t size; /* Number of elements in the array. */
    size_t capacity; /* Number of possible elements in the array. */
} List;


/**
 * @brief Creates a new list of generic type
 * 
 * @param type_size Size of the type used
 * @param capacity Capacity of the initial list
 * @return List* Returns a list handle
 */
List* list_new(size_t type_size, int capacity);

/**
 * @brief Deletes a given list
 * 
 * @param list List to be deleted
 */
void list_delete(List* list);

/**
 * @brief Pushes a new piece of data onto the list
 * 
 * @param list List to be pushed upon
 * @param data Data to be pushed
 */
void list_push(List* list, void* data);

/**
 * @brief Pops the top of the data stack off the list
 * 
 * @param list List to be popped
 */
void list_pop(List* list);

/**
 * @brief Gets a member element of the list at idx returns NULL upon fail
 * 
 * @param list List to be read 
 * @param idx Index requested
 * @return void* Returns either the requested data address or NULL if it fails
 */
void* list_at(List* list, size_t idx);