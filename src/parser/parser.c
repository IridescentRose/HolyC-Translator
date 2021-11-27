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

/*
 * @brief Get the type object
 * 
 * @param slice Slice for a type ID
 * @return Type Type
 *
 */
Type get_primitive(StringSlice slice){
    
    if(strcmp(slice.ptr, "U0") == 0){
        return TYPE_U0;
    }
    if(strcmp(slice.ptr, "U8") == 0){
        return TYPE_U8;
    }
    if(strcmp(slice.ptr, "U16") == 0){
        return TYPE_U16;
    }
    if(strcmp(slice.ptr, "U32") == 0){
        return TYPE_U32;
    }
    if(strcmp(slice.ptr, "U64") == 0){
        return TYPE_U64;
    }
    if(strcmp(slice.ptr, "I0") == 0){
        return TYPE_I0;
    }
    if(strcmp(slice.ptr, "I8") == 0){
        return TYPE_I8;
    }
    if(strcmp(slice.ptr, "I16") == 0){
        return TYPE_I16;
    }
    if(strcmp(slice.ptr, "I32") == 0){
        return TYPE_I32;
    }
    if(strcmp(slice.ptr, "I64") == 0){
        return TYPE_I64;
    }
    if(strcmp(slice.ptr, "F64") == 0){
        return TYPE_F64;
    }

    return -1;
}


void get_primitive_identifier(List* token_list, Token* token, size_t* idx, CType* type, StringSlice* identifier){
    memset(type, 0, sizeof(CType));
    type->primitive = get_primitive(token->slice);

    Token* next_tok = get_next(token_list, idx);

    if(next_tok->slice.ptr[0] == '*'){
        type->is_pointer = 1;
        next_tok = get_next(token_list, idx);
    }
    
    if(next_tok->type != TOKEN_TYPE_IDENTIFIER){
        CHECK_FAILED("Error: Expected identifier after primitive declaration! Found %s at %d:%d!\n", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
    }
    (*identifier) = next_tok->slice;
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
            //TODO: Add call-check support
            make_expression_general(token_list, token->slice, idx, block->statement_list);
            break;
        }

        case TOKEN_TYPE_STRING: {
            make_expression_printf(token_list, token, idx, block->statement_list);
            break;
        }

        case TOKEN_TYPE_PRIMITIVE: {
            CType type;
            StringSlice identifier;

            get_primitive_identifier(token_list, token, idx, &type, &identifier);

            Token* next_tok = get_next(token_list, idx);

            if(next_tok->type == TOKEN_TYPE_TERMINATOR){
                make_variable_declaration(block->statement_list, identifier, type);
                (*idx)++;
            } else if (next_tok->type == TOKEN_TYPE_ASSIGNMENT) {
                make_variable_declaration(block->statement_list, identifier, type);
                make_expression_compound_assign(token_list, identifier, idx, block->statement_list);
            } else if (next_tok->type == TOKEN_TYPE_PUNCTUATOR) {
                //<arguments> <punctuator> <terminator>
                //<arguments> <punctuator> <scoping>
                CHECK_FAILED("PRIMITIVE DETECTED!\n");
            } else {
                CHECK_FAILED("INVALID!\n");
            }
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
