/**
 * @file combine_pass.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Combination pass for Tokenization
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "tokenizer.h"

/**
 * @brief Combines 2 tokens
 * 
 * @param a Token A
 * @param b Token B
 */
void comb_tok(Token* a, Token* b){
    char tmp = a->slice.ptr[0];
    a->slice.ptr = calloc(sizeof(char), 3);
    a->slice.ptr[0] = tmp;
    a->slice.ptr[1] = b->slice.ptr[0];
    a->slice.ptr[2] = 0;
    a->slice.len = 2;
}

/**
 * @brief Combines 3 tokens
 * 
 * @param a Token A
 * @param b Token B
 * @param c Token C
 */
void comb_tok3(Token* a, Token* b, Token* c){
    char tmp = a->slice.ptr[0];
    a->slice.ptr = calloc(sizeof(char), 4);
    a->slice.ptr[0] = tmp;
    a->slice.ptr[1] = b->slice.ptr[0];
    a->slice.ptr[2] = c->slice.ptr[0];
    a->slice.ptr[3] = 0;
    a->slice.len = 2;
}

/**
 * @brief Combines two tokens and adds them to a list
 * 
 * @param a Token A
 * @param b Token B
 * @param list List to add to
 * @param type Type of Token
 * @param idx Index
 */
void combine_set_token(Token* a, Token* b, List* list, TokenType type, size_t* idx) {
    comb_tok(a, b);
    a->type = type;
    list_push(list, a);
    (*idx)++;
}

/**
 * @brief Combines tokens IF they might have a secondary form (+= <=) for example
 * 
 * @param token_list List of tokens to process
 */
List* combine_tokens(List* token_list) {
    size_t idx = 0;
    List* new_list = list_new(sizeof(Token), 1);

    while(idx < token_list->size) {
        Token* token = list_at(token_list, idx);
        char* tmp = token->slice.ptr;

        switch(token->type) {

            case TOKEN_TYPE_RELATION: {
                Token* next_token = list_at(token_list, idx + 1);
                if(next_token){
                    if(next_token->type == TOKEN_TYPE_ASSIGNMENT){
                        combine_set_token(token, next_token, new_list, TOKEN_TYPE_RELATION, &idx);
                        break;
                    }else if(next_token->type == TOKEN_TYPE_RELATION){
                        Token* next_next_token = list_at(token_list, idx + 2);
                        if(next_next_token) {
                            if(next_next_token->type == TOKEN_TYPE_ASSIGNMENT) {
                                comb_tok3(token, next_token, next_next_token);
                                token->type = TOKEN_TYPE_ASSIGNMENT;
                                list_push(new_list, token);
                                idx += 2;
                            } else {
                                combine_set_token(token, next_token, new_list, TOKEN_TYPE_BITWISE, &idx);
                            }
                            break;
                        } else {
                            combine_set_token(token, next_token, new_list, TOKEN_TYPE_BITWISE, &idx);
                            break;
                        }
                    }
                }
                goto explicit_jump;
            }

            case TOKEN_TYPE_BITWISE: {
                Token* next_token = list_at(token_list, idx + 1);
                if(next_token){
                    if(next_token->type == TOKEN_TYPE_ASSIGNMENT){
                        combine_set_token(token, next_token, new_list, TOKEN_TYPE_ASSIGNMENT, &idx);
                        break;
                    }else if(next_token->type == TOKEN_TYPE_BITWISE && next_token->slice.ptr[0] != '~' && next_token->slice.ptr[0] != '^'){
                        combine_set_token(token, next_token, new_list, TOKEN_TYPE_LOGIC, &idx);
                        break;
                    }
                }
                goto explicit_jump;
            }

            case TOKEN_TYPE_LOGIC: {
                Token* next_token = list_at(token_list, idx + 1);
                if(next_token && next_token->type == TOKEN_TYPE_ASSIGNMENT){
                    combine_set_token(token, next_token, new_list, TOKEN_TYPE_RELATION, &idx);
                    break;
                }
                goto explicit_jump;
            }

            

            case TOKEN_TYPE_ASSIGNMENT: {
                Token* next_token = list_at(token_list, idx + 1);
                if(next_token && next_token->type == TOKEN_TYPE_ASSIGNMENT){ // ==
                    combine_set_token(token, next_token, new_list, TOKEN_TYPE_RELATION, &idx);
                    break;
                }
                goto explicit_jump;
            }

            case TOKEN_TYPE_ARITHMETIC: {
                Token* next_token = list_at(token_list, idx + 1);
                if(next_token && next_token->type == TOKEN_TYPE_ASSIGNMENT && next_token->slice.ptr[0] != '%'){
                    combine_set_token(token, next_token, new_list, TOKEN_TYPE_ASSIGNMENT, &idx);
                    break;
                }
                goto explicit_jump;
            }

            default: {
            explicit_jump:
                token->slice.ptr = dupe_string(token->slice.ptr);
                list_push(new_list, token);
                break;
            }
        }

        token->slice.ptr = tmp;
        idx++;
    }

    free_tokens(token_list);
    list_delete(token_list);
    return new_list;
}