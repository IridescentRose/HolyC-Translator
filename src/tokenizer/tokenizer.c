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
                temp = extract_token_preprocessor(content, &idx);
                temp.line = line;
                temp.cursor = cursor;

                if(temp.slice.ptr)
                    list_push(token_list, &temp);
                break;
            }
            
            case '\"': {
                temp = extract_token_string(content, &idx);
                
                temp.line = line;
                temp.cursor = cursor;

                list_push(token_list, &temp);
                break;
            }

            case ';': {
                temp.type = TOKEN_TYPE_TERMINATOR;
                
                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                temp.line = line;
                temp.cursor = cursor;

                list_push(token_list, &temp);
                break;
            }


            case '=': {
                temp.type = TOKEN_TYPE_ASSIGNMENT;

                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                temp.line = line;
                temp.cursor = cursor;

                list_push(token_list, &temp);
                break;
            }

            case '(':
            case ')':
            case ',':
            case '.':
            case '[':
            case ']': {
                temp.type = TOKEN_TYPE_PUNCTUATOR;
                
                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                temp.line = line;
                temp.cursor = cursor;

                list_push(token_list, &temp);
                break;
            }

            case '+':
            case '-':
            case '%': 
            case '*': {
                temp.type = TOKEN_TYPE_ARITHMETIC;
                
                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                temp.line = line;
                temp.cursor = cursor;

                list_push(token_list, &temp);
                break;
            }
            
            case '{':
            case '}': {
                temp.type = TOKEN_TYPE_SCOPING;

                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                temp.line = line;
                temp.cursor = cursor;

                list_push(token_list, &temp);
                break;
            }

            case '/': {
                if(idx + 1 < len){
                    if(content[idx] == content[idx+1]){
                        while(content[idx++] != '\n'){}
                    }else if (content[idx+1] == '*') {
                        idx++;
                        
                        printf("%ld\n", idx);
                        while(!(content[idx] == '*' && content[idx + 1] == '/')){
                            idx++;
                        }
                        idx += 2;
                    }else{
                        temp.type = TOKEN_TYPE_ARITHMETIC;

                        temp.slice.ptr = make_slice(content, idx++, 1);
                        temp.slice.len = 1;
                        temp.line = line;
                        temp.cursor = cursor;

                        list_push(token_list, &temp);
                    }
                }
                break;
            }

            default: {
                if( (content[idx] >= 'a' && content[idx] <= 'z') || (content[idx] >= 'A' && content[idx] <= 'Z') ){
                    temp = extract_token_identifier(content, &idx);
                    
                    temp.line = line;
                    temp.cursor = cursor;
                    
                    list_push(token_list, &temp);
                } else if((content[idx] >= '0' && content[idx] <= '9')) {
                    temp = extract_token_literal(content, &idx);
                    
                    temp.line = line;
                    temp.cursor = cursor;

                    list_push(token_list, &temp);
                } else {
                    CHECK_FAILED("Unhandled token at %d:%d - unknown char %c (%d)\n", line, cursor, content[idx], (int)content[idx]);
                }
                break;
            }
        }
        
    }
}

/**
 * @brief Frees a token list specifically
 * 
 * @param token_list Token list to be freed
 */
void free_tokens(List* token_list) {
    for(size_t i = 0; i < token_list->size; i++){
        Token* tk = list_at(token_list, i);
        free(tk->slice.ptr);
    }
}

/**
 * @brief Checks token against list of primitives
 * 
 * @param tk Token to be checked
 * @return int 0 if not found, 1 if found
 */
int check_primitive(Token* tk){
    char* prims[10] = {"U0", "U8", "U16", "U32", "U64", "I8", "I16", "I32", "I64", "F64"};

    for(size_t i = 0; i < 10; i++){
        if(strcmp(tk->slice.ptr, prims[i]) == 0){
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Validates a token list by scanning identifiers for significant token changes
 * 
 * @param token_list List of tokens to be checked
 */
void validate_identifiers(List* token_list){
    for(size_t i = 0; i < token_list->size; i++){
        Token* tk = list_at(token_list, i);

        if(tk->type == TOKEN_TYPE_IDENTIFIER) {
            if(check_primitive(tk))
                tk->type = TOKEN_TYPE_PRIMITIVE;
            

            if(strcmp("return", tk->slice.ptr) == 0)
                tk->type = TOKEN_TYPE_RETURN;
            
            if(strcmp("extern", tk->slice.ptr) == 0)
                tk->type = TOKEN_TYPE_EXTERN;
            
        }
    }
}

List* tokenize(const char* filename){
    char* content = get_file_contents(filename);
    List* token_list = list_new(sizeof(Token), 1);
    
    process_content(token_list, content);
    free(content);

    //TODO: Delete Validate Identifiers
    validate_identifiers(token_list);
    
    printf("Tokenization Complete: %ld Tokens Generated.\n", token_list->size);
    return token_list;
}