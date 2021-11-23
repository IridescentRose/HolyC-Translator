/**
 * @file combine_pass.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Combination pass for Tokenization
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "../util.h"

/**
 * @brief Combines tokens IF they might have a secondary form (+= <=) for example
 * 
 * @param token_list List of tokens to process
 */
List* combine_tokens(List* token_list);
