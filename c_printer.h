//BY SUBMITTING THIS FILE TO CARMEN, I CERTIFY THAT I HAVE STRICTLY ADHERED
//TO THE TENURES OF THE OHIO STATE UNIVERSITY’S ACADEMIC INTEGRITY POLICY
//WITH RESPECT TO THIS ASSIGNMENT.


/**
 * @file c_printer.h
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Prints a program into C
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "parser.h"

/**
 * @brief Emits C code to a file given a program
 * 
 * @param program Program to be written
 * @param filename File name for output
 */
void emit_c(Program* program, const char* filename);