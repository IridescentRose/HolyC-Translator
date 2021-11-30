#include "conditional.h"


/**
 * @brief Make a conditional if
 */
void make_conditional_if(List* token_list, size_t* idx, List* statement_list, struct ScopeBlock* block, int else_if){

    Token* next_tok = get_next(token_list, idx);

    if(next_tok->slice.ptr[0] != '('){
        CHECK_FAILED("Error: Expected '(' after if statement! Found %s at %d:%d\n", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
    }

    //Next token is first
    next_tok = get_next(token_list, idx);

    //Create expression
    Expression* expr = (Expression*)calloc(1, sizeof(Expression));
    expr->type = EXPRESSION_TYPE_GENERAL;

    //Generate expression
    while(next_tok->slice.ptr[0] != ')') {
        strcat(expr->buffer, next_tok->slice.ptr);
        next_tok = get_next(token_list, idx);
    }

    //Current is a ')' end punctuator - we need next to be scoping
    next_tok = get_next(token_list, idx); 

    if(next_tok->slice.ptr[0] != '{'){
        CHECK_FAILED("Error: Expected '{' after if statement! Found %s at %d:%d\n", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
    }

    next_tok = get_next(token_list, idx); 

    //Create conditional
    Conditional* cond = (Conditional*)calloc(1, sizeof(Conditional));

    if(else_if)
        cond->type = CONDITIONAL_TYPE_ELSE_IF;
    else
        cond->type = CONDITIONAL_TYPE_IF;
    cond->condition = expr;

    //Generate contents
    struct ScopeBlock *statementBlock = (struct ScopeBlock *)calloc(1, sizeof(struct ScopeBlock));
    cond->conditional_content = statementBlock;
    statementBlock->parent = block;
    statementBlock->statement_list = list_new(sizeof(Statement), 32);

    
    Token *test = list_at(token_list, *idx);

    while (test && test->slice.ptr[0] != '}') {
        parse_token_program(statementBlock, token_list, idx);
        test = list_at(token_list, *idx);
    }

    //Create 
    Statement statement;
    statement.type = STATEMENT_TYPE_CONDITIONAL;
    statement.statementData = cond;

    list_push(statement_list, &statement);
    (*idx)++;
}


void make_conditional_else(List* token_list, size_t* idx, List* statement_list, struct ScopeBlock* block){
    Token* next_tok = get_next(token_list, idx);

    if(next_tok->slice.ptr[0] != '{'){
        CHECK_FAILED("Error: Expected '{' after if statement! Found %s at %d:%d\n", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
    }

    next_tok = get_next(token_list, idx); 

    //Create conditional
    Conditional* cond = (Conditional*)calloc(1, sizeof(Conditional));
    cond->type = CONDITIONAL_TYPE_ELSE;

    //Generate contents
    struct ScopeBlock *statementBlock = (struct ScopeBlock *)calloc(1, sizeof(struct ScopeBlock));
    cond->conditional_content = statementBlock;
    statementBlock->parent = block;
    statementBlock->statement_list = list_new(sizeof(Statement), 32);

    
    Token *test = list_at(token_list, *idx);

    while (test && test->slice.ptr[0] != '}') {
        parse_token_program(statementBlock, token_list, idx);
        test = list_at(token_list, *idx);
    }

    //Create 
    Statement statement;
    statement.type = STATEMENT_TYPE_CONDITIONAL;
    statement.statementData = cond;

    list_push(statement_list, &statement);
    (*idx)++;
}