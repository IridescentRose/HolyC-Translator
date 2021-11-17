/**
 * @file parser.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Parser for HolyC
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

 #include "parser.h"

Program* parse(List* token_list){
    Program* program = calloc(1, sizeof(Program));

    printf("%p", (void*)token_list);

    return program;
}