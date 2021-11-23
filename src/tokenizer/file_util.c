/**
 * @file file_util.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief File utilities for tokenization
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "file_util.h"

/**
 * @brief Grabs the length of a file by seeking to end, getting position, then returning to initial position.
 * 
 * @param f Valid File Pointer
 * @return size_t Size of file in bytes (chars)
 */
size_t file_length(FILE* f){
    size_t length = 0;
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    return length;
}

/**
 * @brief Reads a file to a string
 * 
 * @param f Valid File Pointer
 * @return char* File as a string
 */
char* read_file_to_str(FILE* f){
    size_t len = file_length(f);
    char* res = calloc(len + 1, sizeof(char));
    CHECK_NOT_NULL(res, "Error: Allocation of file buffer failed!\n");

    CHECK_GREATER_ZERO(fread(res, 1, len, f), "Error: Unable to read file!\n");

    return res;
}

/**
 * @brief Get the file contents
 * 
 * @param filename File to open
 * @return char* File contents as a string
 */
char* get_file_contents(const char* filename){
    FILE* fp = fopen(filename, "r");
    CHECK_NOT_NULL(fp, "Error: File could not be opened!\n");
    
    char* file_str = read_file_to_str(fp);
    fclose(fp);

    return file_str;
}
