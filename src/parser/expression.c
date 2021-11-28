#include "expression.h"


void make_expression_general(List* token_list, StringSlice identifier, size_t* idx, List* statement_list, char* prepend){
    Expression* expr = (Expression*)calloc(1, sizeof(Expression));
    expr->type = EXPRESSION_TYPE_GENERAL;

    strcat(expr->buffer, identifier.ptr);
    if(prepend) {
        strcat(expr->buffer, prepend);
    }
    
    //Finish 
    Token* next_tok = get_next(token_list, idx);
    while(next_tok->type != TOKEN_TYPE_TERMINATOR){
        strcat(expr->buffer, next_tok->slice.ptr);
        next_tok = get_next(token_list, idx);
    }

    //Create statement
    Statement statement;
    statement.type = STATEMENT_TYPE_EXPRESSION;
    statement.statementData = expr;

    //Add to list
    list_push(statement_list, &statement);
    (*idx)++;
}

void make_expression_call(StringSlice identifier, List* statement_list){
    Expression* expr = (Expression*)calloc(1, sizeof(Expression));
    expr->type = EXPRESSION_TYPE_CALL;
    strcat(expr->buffer, identifier.ptr);
    
    //Create statement
    Statement statement;
    statement.type = STATEMENT_TYPE_EXPRESSION;
    statement.statementData = expr;

    //Add to list
    list_push(statement_list, &statement);
}

void make_expression_compound_assign(List* token_list, StringSlice identifier, size_t* idx, List* statement_list){
    Expression* expr = (Expression*)calloc(1, sizeof(Expression));
    expr->type = EXPRESSION_TYPE_GENERAL;

    strcat(expr->buffer, identifier.ptr);
    strcat(expr->buffer, " = ");
    
    //Finish 
    Token* next_tok = get_next(token_list, idx);
    while(next_tok->type != TOKEN_TYPE_TERMINATOR){
        strcat(expr->buffer, next_tok->slice.ptr);
        next_tok = get_next(token_list, idx);
    }

    //Create statement
    Statement statement;
    statement.type = STATEMENT_TYPE_EXPRESSION;
    statement.statementData = expr;

    //Add to list
    list_push(statement_list, &statement);
    (*idx)++;
}

void make_expression_printf(List* token_list, Token* token, size_t* idx, List* statement_list){
    Expression* expr = (Expression*)calloc(1, sizeof(Expression));
    expr->type = EXPRESSION_TYPE_PRINTF;

    //If it's not blank, copy it.
    if(token->slice.len > 2) {
        strcat(expr->buffer, token->slice.ptr);
    }
    
    //Finish 
    Token* next_tok = get_next(token_list, idx);
    while(next_tok->type != TOKEN_TYPE_TERMINATOR){
        strcat(expr->buffer, next_tok->slice.ptr);
        next_tok = get_next(token_list, idx);
    }

    //Create statement
    Statement statement;
    statement.type = STATEMENT_TYPE_EXPRESSION;
    statement.statementData = expr;

    //Add to list
    list_push(statement_list, &statement);
    (*idx)++;
}
