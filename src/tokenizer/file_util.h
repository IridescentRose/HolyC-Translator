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
#include "../util.h"

/**
 * @brief Get the file contents
 * 
 * @param filename File to open
 * @return char* File contents as a string
 */
char* get_file_contents(const char* filename);
