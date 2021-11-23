/**
 * @file statement.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Creates statements
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "statement.h"

void make_variable_declaration(List* list, StringSlice identifier, Type type, char pointer, char externf) {
    Statement statement;
    statement.type = STATEMENT_TYPE_DECLARATION;

    Declaration* decl = (Declaration*)calloc(1, sizeof(Declaration));
    decl->type = type;
    decl->identifier = identifier;
    decl->is_function = 0;
    decl->pointer = pointer;
    decl->externf = externf;

    statement.statementData = decl;
    list_push(list, &statement);
}

void make_function_declaration(List* list, StringSlice identifier, Type type, char pointer, char externf, Arguments args) {
    Statement statement;
    statement.type = STATEMENT_TYPE_DECLARATION;

    Declaration* decl = (Declaration*)calloc(1, sizeof(Declaration));
    decl->type = type;
    decl->identifier = identifier;
    decl->is_function = 1;
    decl->pointer = pointer;
    decl->args = args;
    decl->externf = externf;

    statement.statementData = decl;
    list_push(list, &statement);
}

void make_function_definition(List* list, StringSlice identifier, Type type, char pointer, Arguments args, struct ScopeBlock* statement_list) {
    Statement statement;
    statement.type = STATEMENT_TYPE_DEFINITION;

    Definition* def = (Definition*)calloc(1, sizeof(Definition));
    def->type = type;
    def->identifier = identifier;
    def->is_function = 1;
    def->pointer = pointer;
    def->args = args;
    def->function_content = statement_list;

    statement.statementData = def;
    list_push(list, &statement);
}
