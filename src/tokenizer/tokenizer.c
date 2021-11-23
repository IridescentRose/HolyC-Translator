/**
 * @file tokenizer.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Basic tokenizer for HolyC
 * @version 1.0
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "tokenizer.h"
#include "extractor.h"
#include "combine_pass.h"

/**
 * @brief Loops through a content string to process tokens
 * 
 * @param token_list List of tokens to create
 * @param content String to interpret
 */
void process_content(List* token_list, char* content){
    size_t idx = 0;
    size_t len = strlen(content);

    int line = 0;
    int cursor = 0;

    while(idx < len){
        Token temp;

        switch(content[idx]) {
            case ' ': {
                cursor++;
                idx++;
                break;
            }

            case '\t': {
                cursor += 4;
                idx++;
                break;
            }

            case '\n': {
                line++;
                cursor = 0;
                idx++;
                break;
            }

            case '#': {
                extract_token_preprocessor(content, &idx, line, &cursor, token_list);
                line += 1;
                cursor = 0;
                break;
            }
            
            case '\"': {
                extract_token_string(content, &idx, line, &cursor, token_list);
                break;
            }

            case ';': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_TERMINATOR, line, &cursor, token_list);
                break;
            }


            case '=': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_ASSIGNMENT, line, &cursor, token_list);
                break;
            }

            case '&':
            case '|':
            case '^':
            case '~': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_BITWISE, line, &cursor, token_list);
                break;
            }

            case '!': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_LOGIC, line, &cursor, token_list);
                break;
            }

            case '?':
            case ':': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_MISC, line, &cursor, token_list);
                break;
            }

            case '>':
            case '<': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_RELATION, line, &cursor, token_list);
                break;

            }

            case '(':
            case ')':
            case ',':
            case '.':
            case '[':
            case ']': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_PUNCTUATOR, line, &cursor, token_list);
                break;
            }

            case '+':
            case '-':
            case '%': 
            case '*': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_ARITHMETIC, line, &cursor, token_list);
                break;
            }
            
            case '{':
            case '}': {
                extract_token_single_char(content, &idx, TOKEN_TYPE_SCOPING, line, &cursor, token_list);
                break;
            }

            case '/': {
                if(idx + 1 < len){
                    if(content[idx] == content[idx+1]){
                        while(content[idx++] != '\n'){cursor++;}
                        cursor++;
                    }else if (content[idx+1] == '*') {
                        idx++;
                        cursor++;
                        while(!(content[idx] == '*' && content[idx + 1] == '/')){
                            idx++;
                            cursor++;
                        }
                        cursor += 2;
                        idx += 2;
                    }else{
                        extract_token_single_char(content, &idx, TOKEN_TYPE_ARITHMETIC, line, &cursor, token_list);
                    }
                }
                break;
            }

            default: {
                if( (content[idx] >= 'a' && content[idx] <= 'z') || (content[idx] >= 'A' && content[idx] <= 'Z') )
                    temp = extract_token_identifier(content, &idx, line, &cursor);
                else if((content[idx] >= '0' && content[idx] <= '9'))
                    temp = extract_token_literal(content, &idx, line, &cursor);
                else {
                    CHECK_FAILED("Unhandled token at %d:%d - unknown char %c (%d)\n", line, cursor, content[idx], (int)content[idx]);
                }

                list_push(token_list, &temp);
                break;
            }
        }
        
    }
}

List* tokenize(const char* filename){
    char* content = get_file_contents(filename);
    List* token_list = list_new(sizeof(Token), 1);
    
    process_content(token_list, content);
    free(content);
    
    printf("Tokenizer: Complete, %ld Tokens Generated.\n", token_list->size);

    token_list = combine_tokens(token_list);
    return token_list;
}

void free_tokens(List* token_list) {
    for(size_t i = 0; i < token_list->size; i++){
        Token* tk = list_at(token_list, i);
        free(tk->slice.ptr);
    }
}
