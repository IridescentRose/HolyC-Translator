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

void make_variable_declaration(Program* program, StringSlice identifier, Type type, char pointer) {
    Statement statement;
    statement.type = STATEMENT_TYPE_DECLARATION;

    Declaration* decl = (Declaration*)calloc(1, sizeof(Declaration));
    decl->type = type;
    decl->identifier = identifier;
    decl->is_function = 0;
    decl->pointer = pointer;

    statement.statementData = decl;
    list_push(program->block.statement_list, &statement);
}

Type get_type(StringSlice slice){
    
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

void parse_token_program(Program* program, List* token_list, size_t* idx){
    Token* token = list_at(token_list, *idx);
    //Token* next_token = list_at(token_list, *idx + 1);

    switch(token->type){
        case TOKEN_TYPE_PREPROCESSOR: {
            make_preprocessor(program, token);
            (*idx)++;

            break;
        }

        case TOKEN_TYPE_PRIMITIVE: {
            //Upon receiving a primitive, we have to check what it is.
            Type type = get_type(token->slice);

            (*idx)++;
            Token* next_tok = list_at(token_list, *idx);
            CHECK_NOT_NULL(next_tok, "Error: Unexpected end of file!");

            char pointer = 0;
            if(next_tok->type == TOKEN_TYPE_ARITHMETIC && next_tok->slice.ptr[0] == '*'){
                //It's a pointer
                pointer = 1;
                
                (*idx)++;
                next_tok = list_at(token_list, *idx);
                CHECK_NOT_NULL(next_tok, "Error: Unexpected end of file!");
            }
            
            if(next_tok->type != TOKEN_TYPE_IDENTIFIER){
                CHECK_FAILED("Error: Expected Identifier After Primitive!\n");
            }
            
            StringSlice identifier = next_tok->slice;

            (*idx)++;
            next_tok = list_at(token_list, *idx);
            CHECK_NOT_NULL(next_tok, "Error: Unexpected end of file!");

            if(next_tok->type == TOKEN_TYPE_PUNCTUATOR){
                //This is a function
            } else if (next_tok->type == TOKEN_TYPE_TERMINATOR){
                //This was a variable
                make_variable_declaration(program, identifier, type, pointer);
                (*idx)++;
            } else if (next_tok->type == TOKEN_TYPE_ASSIGNMENT){
                //This is a fused Declaration + Assignment
                CHECK_FAILED("TODO: COMPOUND DECLARATION & ASSIGNMENT");
            }

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

            case STATEMENT_TYPE_DECLARATION:{
                Declaration* decl = (Declaration*)st->statementData;
                free(decl);
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