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


void make_variable_declaration_assign(List* token_list, size_t* idx, StringSlice identifier, CType type, List* statement_list){

    Expression* expr = (Expression*)calloc(1, sizeof(Expression));
    expr->type = EXPRESSION_TYPE_GENERAL;
    strcat(expr->buffer, "= ");
    
    //Finish 
    Token* next_tok = get_next(token_list, idx);
    while(next_tok->type != TOKEN_TYPE_TERMINATOR){
        strcat(expr->buffer, next_tok->slice.ptr);
        next_tok = get_next(token_list, idx);
    }

    Statement statement;
    statement.type = STATEMENT_TYPE_DECLARATION;

    Declaration* decl = (Declaration*)calloc(1, sizeof(Declaration));
    decl->type = type;
    decl->identifier = identifier;
    decl->is_function = 0;
    decl->expr = expr;

    statement.statementData = decl;

    
    list_push(statement_list, &statement);

    (*idx)++;
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
