#include "declaration.h"

void make_variable_declaration(List* list, StringSlice identifier, CType ctype) {
    Statement statement;
    statement.type = STATEMENT_TYPE_DECLARATION;

    Declaration* decl = (Declaration*)calloc(1, sizeof(Declaration));
    decl->type = ctype;
    decl->identifier = identifier;
    decl->is_function = 0;

    statement.statementData = decl;
    list_push(list, &statement);
}

void make_function_declaration(List* list, StringSlice identifier, CType ctype, Arguments args) {
    Statement statement;
    statement.type = STATEMENT_TYPE_DECLARATION;

    Declaration* decl = (Declaration*)calloc(1, sizeof(Declaration));
    decl->type = ctype;
    decl->identifier = identifier;
    decl->is_function = 1;
    decl->args = args;

    statement.statementData = decl;
    list_push(list, &statement);
}
