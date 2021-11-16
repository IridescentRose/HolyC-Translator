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


char* make_slice(const char* str, size_t idx, size_t len){
    char* res = calloc(len + 1, sizeof(char));

    for(size_t i = 0; i < len; i++){
        res[i] = str[idx + i];
    }

    return res;
}

/**
 * @brief Inits a new list of generic type
 * 
 * @param list List to be initialized
 * @param type_size Size of the type used
 * @param capacity Capacity of the initial list
 */
void list_init(List* list, size_t type_size, int capacity){
    list->data = calloc(type_size, capacity);

    CHECK_NOT_NULL(list->data, "Error: Could not allocate list internal!\n");

    list->type_size = type_size;
    list->capacity = capacity;
    list->size = 0;
}


List* list_new(size_t type_size, int capacity) {
    List* list = (List*)calloc(sizeof(List), 1);
    CHECK_NOT_NULL(list, "Error: Could not allocate list!\n");

    list_init(list, type_size, capacity);
    return list;
}

/**
 * @brief Cleans up list internals
 * 
 * @param list List to be cleaned
 */
void list_cleanup(List* list){
    if(list->data){
        free(list->data);
        list->data = NULL;
    }

    list->size = 0;
    list->type_size = 0;
    list->capacity = 0;
}

void list_delete(List* list){
    list_cleanup(list);
    free(list);
}

void list_push(List* list, void* data){

    if(list->size >= list->capacity){
        /* Resize */
        void* temp = realloc(list->data, 2 * list->capacity * list->type_size);
        CHECK_NOT_NULL(temp, "Error: List resize failed!\n");
        
        list->data = temp;
        list->capacity *= 2;
    }

    memcpy(&((char*)list->data)[list->size * (unsigned long)list->type_size], data, list->type_size);
    list->size++;
}

void list_pop(List* list) {
    if(list->size > 0){
        list->size--;
    }
}

void* list_at(List* list, size_t idx){
    if(idx < list->size){
        return ((char*)list->data) + idx * list->type_size;
    }

    return NULL;
}
