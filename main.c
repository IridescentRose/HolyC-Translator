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
#include "tokenizer.h"

char* g_input_name = NULL;
char* g_output_name = NULL;

/**
 * @brief Print and request input file name from user.
 * 
 */
void request_input(){
    int items_read = 0;
    char buffer[16] = {0};

    printf("Please enter your input file name: ");
    items_read = scanf("%s", buffer);
    CHECK_GREATER_ZERO(items_read, "Error: Could not read input file name to buffer!\n");
    g_input_name = dupe_string(buffer);
}

/**
 * @brief Print and request output file name from user.
 * 
 */
void request_output(){
    int items_read = 0;
    char buffer[16] = {0};

    printf("Please enter your output file name: ");
    items_read = scanf("%s", buffer);
    CHECK_GREATER_ZERO(items_read, "Error: Could not read output file name to buffer!\n");
    g_output_name = dupe_string(buffer);
}

/**
 * @brief Checks argument count if the provided arguments are present. If not, the program prompts for a response
 * 
 * @param argc Argument Count
 * @param argv Argument Variables
 */
void check_args(int argc, char** argv){
    if(argc < 3){
        /* We have to request */
        printf("You did not input at least 2 variables!\nThe correct usage of the command is: hc2c <input file name> <output file name>\n\n");
        request_input();
        request_output();
    } else {
        g_input_name = dupe_string(argv[1]);
        g_output_name = dupe_string(argv[2]);
    }
}

/**
 * @brief Frees the global input and output name variables at the end of execution.
 * 
 */
void free_names() {
    free(g_input_name);
    free(g_output_name);
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

    tokenize(g_input_name);

    free_names();
    return EXIT_SUCCESS;
}