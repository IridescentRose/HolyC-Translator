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
#include "preprocessor.h"
#include "statement.h"

/**
 * @brief Checks if an identifier is a function
 * 
 * @param block Scoped Block to check from
 * @param identifier Identifier to look for
 * @return int 0 if cannot be found, 1 if could find and is a function
 */
int check_identifier_function(struct ScopeBlock* block, StringSlice identifier){

    for(size_t i = 0; i < block->statement_list->size; i++){
        Statement* statement = (Statement*)list_at(block->statement_list, i);

        if(statement->type == STATEMENT_TYPE_DEFINITION || statement->type == STATEMENT_TYPE_DECLARATION){
            //Could be
            //We can coerce to Declaration type
            Declaration* decl = (Declaration*)statement->statementData;
            
            if(decl->is_function){
                if(strcmp(decl->identifier.ptr, identifier.ptr) == 0){
                    return 1;
                }
            }
        }
    }

    if(block->parent){
        return check_identifier_function(block->parent, identifier);
    }
    return 0;
}

/**
 * @brief Get the type object
 * 
 * @param slice Slice for a type ID
 * @return Type Type
 */
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

/**
 * @brief Get the next object
 * 
 * @param token_list Token List
 * @param idx Index to increments
 * @return Token* Next token
 */
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

    int externf = 0;
    switch(token->type){
        case TOKEN_TYPE_PREPROCESSOR: {
            make_preprocessor(block->statement_list, token, idx);
            break;
        }

        case TOKEN_TYPE_SCOPING: {
            break;
        }

        case TOKEN_TYPE_IDENTIFIER: {
            StringSlice identifier = token->slice;

            Expression* expr = (Expression*)calloc(1, sizeof(Expression));
            strcat(expr->buffer, identifier.ptr);

            Token* next_tok = get_next(token_list, idx);
            
            if(next_tok->type == TOKEN_TYPE_PUNCTUATOR) {
                //Is in format of function call already - we can set as general
                //TODO: make flag for default reordering
                expr->type = EXPRESSION_TYPE_GENERAL;
                while(next_tok->type != TOKEN_TYPE_TERMINATOR){
                    strcat(expr->buffer, next_tok->slice.ptr);
                    next_tok = get_next(token_list, idx);
                }
            } else if(check_identifier_function(block, identifier)) {
                //Is a function call with default args
                expr->type = EXPRESSION_TYPE_CALL;
            } else {
                expr->type = EXPRESSION_TYPE_GENERAL;
                while(next_tok->type != TOKEN_TYPE_TERMINATOR){
                    strcat(expr->buffer, next_tok->slice.ptr);
                    next_tok = get_next(token_list, idx);
                }
            }

            
            (*idx)++;
            Statement statement;
            statement.type = STATEMENT_TYPE_EXPRESSION;
            statement.statementData = expr;

            list_push(block->statement_list, &statement);
            
            break;
        }

        case TOKEN_TYPE_STRING: {
            StringSlice identifier = token->slice;

            Expression* expr = (Expression*)calloc(1, sizeof(Expression));
            expr->type = EXPRESSION_TYPE_PRINTF;

            strcat(expr->buffer, identifier.ptr);

            Token* next_tok = get_next(token_list, idx);
            while(next_tok->type != TOKEN_TYPE_TERMINATOR){
                strcat(expr->buffer, next_tok->slice.ptr);
                next_tok = get_next(token_list, idx);
            }
            (*idx)++;

            Statement statement;
            statement.type = STATEMENT_TYPE_EXPRESSION;
            statement.statementData = expr;

            list_push(block->statement_list, &statement);

            break;
        }

        case TOKEN_TYPE_PRIMITIVE: {
            //Upon receiving a primitive, we have to check what it is.
            Type type = get_type(token->slice);
            Token* next_tok = get_next(token_list, idx);

            char pointer = 0;
            if(next_tok->type == TOKEN_TYPE_ARITHMETIC && next_tok->slice.ptr[0] == '*'){
                pointer = 1;
                next_tok = get_next(token_list, idx);
            }
            
            if(next_tok->type != TOKEN_TYPE_IDENTIFIER){
                CHECK_FAILED("Error: Expected Identifier After Primitive! Found %s instead at %d:%d!\n", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
            }
            
            StringSlice identifier = next_tok->slice;
            next_tok = get_next(token_list, idx);

            if(next_tok->type == TOKEN_TYPE_PUNCTUATOR){
                //This is a function

                next_tok = get_next(token_list, idx);
                
                Arguments args;
                memset(&args, 0, sizeof(Arguments));
                for(int i = 0; i < 16; i++){
                    args.types[i] = -1;
                }

                int count = 0;

                while(next_tok->slice.ptr[0] != ')'){
                    //Gather args data

                    if(next_tok->type == TOKEN_TYPE_PUNCTUATOR){
                        next_tok = get_next(token_list, idx);
                    }

                    if(next_tok->type == TOKEN_TYPE_PRIMITIVE){
                        args.types[count] = get_type(next_tok->slice);
                    } else {
                        if(next_tok->slice.ptr[0] == '.'){
                            next_tok = get_next(token_list, idx);
                            next_tok = get_next(token_list, idx);
                        }else{
                            CHECK_FAILED("Error: Expected Primitive after punctuator! Got %d %s at %d:%d!\n", next_tok->type, next_tok->slice.ptr, next_tok->line, next_tok->cursor);
                        }
                    }

                    next_tok = get_next(token_list, idx);

                    if(next_tok->type == TOKEN_TYPE_ARITHMETIC && next_tok->slice.ptr[0] == '*'){
                        //Pointer
                        args.pointer[count] = 1;
                        next_tok = get_next(token_list, idx);
                    }

                    //Next type can either be an ID or a punctuator
                    if(next_tok->type == TOKEN_TYPE_IDENTIFIER){
                        args.identifiers[count++] = next_tok->slice.ptr;
                        next_tok = get_next(token_list, idx);
                    } else {
                        args.identifiers[count++] = NULL;
                    }

                }

                next_tok = get_next(token_list, idx);
                if(next_tok->type == TOKEN_TYPE_TERMINATOR) {
                    make_function_declaration(block->statement_list, identifier, type, pointer, externf, args);
                } else if(next_tok->type == TOKEN_TYPE_SCOPING) {
                    //Function definition;
                    struct ScopeBlock* statementBlock = (struct ScopeBlock*)calloc(1, sizeof(struct ScopeBlock));
                    statementBlock->parent = block;
                    statementBlock->statement_list = list_new(sizeof(Statement), 32);

                    (*idx)++;
                    Token* test = list_at(token_list, *idx);
                    
                    while(test && test->type != TOKEN_TYPE_SCOPING){
                        parse_token_program(statementBlock, token_list, idx);
                        test = list_at(token_list, *idx);
                    }
                    
                    make_function_definition(block->statement_list, identifier, type, pointer, args, statementBlock);
                }
            } else if (next_tok->type == TOKEN_TYPE_TERMINATOR){
                make_variable_declaration(block->statement_list, identifier, type, pointer, externf);
            } else if (next_tok->type == TOKEN_TYPE_ASSIGNMENT){
                //This is a fused Declaration + Assignment
                make_variable_declaration(block->statement_list, identifier, type, pointer, externf);
                
                Expression* expr = (Expression*)calloc(1, sizeof(Expression));
                strcat(expr->buffer, identifier.ptr);
                strcat(expr->buffer, next_tok->slice.ptr);

                next_tok = get_next(token_list, idx);
                while(next_tok->type != TOKEN_TYPE_TERMINATOR){
                    strcat(expr->buffer, next_tok->slice.ptr);
                    next_tok = get_next(token_list, idx);
                }

                Statement statement;
                statement.type = STATEMENT_TYPE_EXPRESSION;
                statement.statementData = expr;

                list_push(block->statement_list, &statement);
            }
            (*idx)++;

            break;
        }

        default: {
            CHECK_FAILED("Error: Unknown Token %d %s found at %d:%d\n", token->type, token->slice.ptr, token->line, token->cursor);
            break;
        }
    }
}


/**
 * @brief See header
 */
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


/**
 * @brief Frees all memory associated to a program block
 * 
 * @param block Block to free
 */
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