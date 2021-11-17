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

void validate_preprocessor(Token* token) {
    char* res1 = strstr(token->slice.ptr, "#ifaot");
    char* res2 = strstr(token->slice.ptr, "#ifjit");

    if(res1 || res2){
        free(token->slice.ptr);
        token->slice.ptr = dupe_string("#if 0");
        token->slice.len = 6;
    }
}

void make_preprocessor(Program* program, Token* token) {
    validate_preprocessor(token);
    Statement statement;
    statement.type = STATEMENT_TYPE_PREPROCESSOR;

    PreProcessor* preproc = (PreProcessor*)calloc(1, sizeof(PreProcessor));
    preproc->text = token->slice;

    statement.statementData = preproc;
    list_push(program->block.statement_list, &statement);
}

void parse_token_program(Program* program, List* token_list, size_t* idx){
    Token* token = list_at(token_list, *idx);
    //Token* next_token = list_at(token_list, *idx + 1);

    switch(token->type){
        case TOKEN_TYPE_PREPROCESSOR: {
            make_preprocessor(program, token);
            (*idx)++;

            break;
        }

        case TOKEN_TYPE_NEWLINE:{
            (*idx)++;
            break;
        }

        default: {
            CHECK_FAILED("BAD TOKEN %d!\n", token->type);
            break;
        }
    }
}


void free_program(Program* program){

    for(size_t i = 0; i < program->block.statement_list->size; i++){
        Statement* st = list_at(program->block.statement_list, i);

        switch(st->type){
            case STATEMENT_TYPE_PREPROCESSOR:{
                PreProcessor* preproc = (PreProcessor*)st->statementData;
                free(preproc);
                break;
            }

            default: {
                free(st->statementData);
                break;
            }
        }
    }

    list_delete(program->block.statement_list);
    free(program);
}

Program* parse(List* token_list){
    Program* program = calloc(1, sizeof(Program));
    program->block.statement_list = list_new(sizeof(Statement), 32);

    size_t idx = 0; 
    while(idx < token_list->size){
        parse_token_program(program, token_list, &idx);
    }

    return program;
}