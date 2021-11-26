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

Token* get_next(List* token_list, size_t* idx){
    (*idx)++;
    Token* next_tok = list_at(token_list, *idx);
    CHECK_NOT_NULL(next_tok, "Error: Unexpected end of file!");

    return next_tok;
}

/**
 * @brief Parse tokens into a program scope block
 * 
 * This is an extremely complicated method.
 * 
 * @param block Block to process into
 * @param token_list List of all tokens
 * @param idx Index of current token 
 */
void parse_token_program(struct ScopeBlock* block, List* token_list, size_t* idx){
    Token* token = list_at(token_list, *idx);

    switch(token->type){
        case TOKEN_TYPE_PREPROCESSOR: {
            make_preprocessor(block->statement_list, token, idx);
            break;
        }

        case TOKEN_TYPE_SCOPING: {
            //TODO: Support arbitrary scoped blocks
            break;
        }

        case TOKEN_TYPE_IDENTIFIER: {
            CHECK_FAILED("IDENTIFIER DETECTED!\n");
            break;
        }

        case TOKEN_TYPE_STRING: {
            make_expression_printf(token_list, token, idx, block->statement_list);
            break;
        }

        case TOKEN_TYPE_PRIMITIVE: {
            CHECK_FAILED("PRIMITIVE DETECTED!\n");
            break;
        }

        case TOKEN_TYPE_KEYWORD: {
            CHECK_FAILED("KEYWORD DETECTED!\n");
            break;
        }

        default: {
            CHECK_FAILED("Error: Unknown Token %d %s found at %d:%d\n", token->type, token->slice.ptr, token->line, token->cursor);
            break;
        }
    }
}

Program* parse(List* token_list){
    Program* program = calloc(1, sizeof(Program));
    program->block.statement_list = list_new(sizeof(Statement), 32);

    size_t idx = 0; 
    while(idx < token_list->size){
        parse_token_program(&program->block, token_list, &idx);
    }

    printf("Parser: AST Generated!\nParser: Main scope block contains %ld statements!\n", program->block.statement_list->size);
    return program;
}

void free_program(struct ScopeBlock* block){

    for(size_t i = 0; i < block->statement_list->size; i++){
        Statement* st = list_at(block->statement_list, i);

        switch(st->type){
            case STATEMENT_TYPE_PREPROCESSOR:{
                PreProcessor* preproc = (PreProcessor*)st->statementData;
                free(preproc);
                break;
            }

            case STATEMENT_TYPE_DECLARATION:{
                Declaration* decl = (Declaration*)st->statementData;
                free(decl);
                break;
            }
            
            case STATEMENT_TYPE_DEFINITION:{
                Definition* def = (Definition*)st->statementData;
                free_program(def->function_content);
                free(def->function_content);
                free(def);
                break;
            }

            case STATEMENT_TYPE_EXPRESSION:{
                Expression* expr = (Expression*)st->statementData;
                free(expr);
                break;
            }

            default: {
                free(st->statementData);
                break;
            }
        }
    }

    list_delete(block->statement_list);
}
