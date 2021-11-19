//BY SUBMITTING THIS FILE TO CARMEN, I CERTIFY THAT I HAVE STRICTLY ADHERED
//TO THE TENURES OF THE OHIO STATE UNIVERSITY’S ACADEMIC INTEGRITY POLICY
//WITH RESPECT TO THIS ASSIGNMENT.


/**
 * @file main.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Main file
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "c_printer.h"

char* g_input_name = NULL;
char* g_output_name = NULL;

/**
 * @brief Checks argument count if the provided arguments are present. If not, the program prompts for a response
 * 
 * @param argc Argument Count
 * @param argv Argument Variables
 */
void check_args(int argc, char** argv){
    if(argc < 3){
        CHECK_FAILED("Error: Program needs at least 2 inputs!\nUsage: hc2c <input> <output>\n\n");
    } else {
        g_input_name = dupe_string(argv[1]);
        g_output_name = dupe_string(argv[2]);
    }
}

/**
 * @brief The main function of the HolyC to C translator
 * 
 * @param argc Argument Count
 * @param argv Argument Variables
 * @return int - Exit Code
 */
int main(int argc, char** argv){
    check_args(argc, argv);

    List* token_list = tokenize(g_input_name);
    Program* program = parse(token_list);

    emit_c(program, g_output_name);

    
    free_program(&program->block);
    free(program);
    free_tokens(token_list);
    free(g_input_name);
    free(g_output_name);

    return EXIT_SUCCESS;
}