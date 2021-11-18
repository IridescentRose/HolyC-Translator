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

/**
 * @brief Grabs the length of a file by seeking to end, getting position, then returning to initial position.
 * 
 * @param f Valid File Pointer
 * @return size_t Size of file in bytes (chars)
 */
size_t file_length(FILE* f){
    size_t length = 0;
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    return length;
}

/**
 * @brief Reads a file to a string
 * 
 * @param f Valid File Pointer
 * @return char* File as a string
 */
char* read_file_to_str(FILE* f){
    size_t len = file_length(f);
    char* res = calloc(len + 1, sizeof(char));
    CHECK_NOT_NULL(res, "Error: Allocation of file buffer failed!\n");

    CHECK_GREATER_ZERO(fread(res, 1, len, f), "Error: Unable to read file!\n");

    return res;
}

/**
 * @brief Get the file contents
 * 
 * @param filename File to open
 * @return char* File contents as a string
 */
char* get_file_contents(const char* filename){
    FILE* fp = fopen(filename, "r");
    CHECK_NOT_NULL(fp, "Error: File could not be opened!\n");
    
    char* file_str = read_file_to_str(fp);
    fclose(fp);

    return file_str;
}

/**
 * @brief Extract a Token for a preprocessor directive.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @return Returns a Token
 */
Token extract_token_preprocessor(char* content, size_t* idx){
    Token tk;
    tk.slice.len = 0;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_PREPROCESSOR;
    size_t len = strlen(content);

    for(;content[tk.slice.len + *idx] != '\n' && (tk.slice.len + *idx) < len; tk.slice.len++){}

    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    *idx += tk.slice.len;
    return tk;
}

/**
 * @brief Extract a Token for an identifier.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @return Returns a Token
 */
Token extract_token_identifier(char* content, size_t* idx){
    Token tk;
    tk.slice.len = 0;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_IDENTIFIER;

    while(1){
        char s = content[tk.slice.len + *idx];

        if(!((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9'))){
            break;
        }else{
            tk.slice.len++;
        }
    }
    
    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    *idx += tk.slice.len;
    return tk;
}

Token extract_token_literal(char* content, size_t* idx){
    Token tk;
    tk.slice.len = 0;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_IMMEDIATE;

    while(1){
        char s = content[tk.slice.len + *idx];

        if(!((s >= '0' && s <= '9') || s == '.')){
            break;
        }else{
            tk.slice.len++;
        }
    }
    
    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    *idx += tk.slice.len;
    return tk;
}

/**
 * @brief Extract a Token for a string literal.
 * 
 * @param content String to extract from 
 * @param idx*Index pointer to increment
 * @return Returns a Token
 */
Token extract_token_string(char* content, size_t* idx){
    Token tk;
    tk.slice.len = 1;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_STRING;

    for(;content[tk.slice.len + *idx] != '\"'; tk.slice.len++){}
    tk.slice.len++;

    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    *idx += tk.slice.len;
    return tk;
}

/**
 *
 * @param token_list List of tokens to create
 * @param content String to interpret
 */
void process_content(List* token_list, char* content){
    size_t idx = 0;
    size_t len = strlen(content);
    while(idx < len){
        Token temp;

        switch(content[idx]) {
            case ' ':
            case '\n':
            case '\t': {
                idx++; 
                break;
            }

            case '#': {
                temp = extract_token_preprocessor(content, &idx);
                list_push(token_list, &temp);
                break;
            }
            
            case '\"': {
                temp = extract_token_string(content, &idx);
                list_push(token_list, &temp);
                break;
            }

            case ';': {
                temp.type = TOKEN_TYPE_TERMINATOR;
                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                list_push(token_list, &temp);
                break;
            }

            case '(':
            case ')':
            case ',':
            case '[':
            case ']': {
                temp.type = TOKEN_TYPE_PUNCTUATOR;
                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
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
                list_push(token_list, &temp);
                break;
            }
            
            case '{':
            case '}': {
                temp.type = TOKEN_TYPE_SCOPING;
                temp.slice.ptr = make_slice(content, idx++, 1);
                temp.slice.len = 1;
                list_push(token_list, &temp);
                break;
            }

            case '/': {
                if(idx + 1 < len){
                    if(content[idx] == content[idx+1]){
                        while(content[idx++] != '\n'){}
                    }else{
                        temp.type = TOKEN_TYPE_ARITHMETIC;
                        temp.slice.ptr = make_slice(content, idx++, 1);
                        temp.slice.len = 1;
                        list_push(token_list, &temp);
                    }
                }
                break;
            }

            default: {
                if( (content[idx] >= 'a' && content[idx] <= 'z') || (content[idx] >= 'A' && content[idx] <= 'Z') ){
                    temp = extract_token_identifier(content, &idx);
                    list_push(token_list, &temp);
                } else if((content[idx] >= '0' && content[idx] <= '9')) {
                    temp = extract_token_literal(content, &idx);
                    list_push(token_list, &temp);
                } else {
                    CHECK_FAILED("UNHANDLED %c %d\n", content[idx], (int)content[idx]);
                }
                break;
            }
        }
        
    }
}

void free_tokens(List* token_list) {
    for(size_t i = 0; i < token_list->size; i++){
        Token* tk = list_at(token_list, i);
        free(tk->slice.ptr);
    }
}

void print_tokens(List* token_list) {
    for(size_t i = 0; i < token_list->size; i++){
        Token* tk = list_at(token_list, i);
        
        if(tk->type != 13){
            printf("TOKEN %ld: { TYPE: %d, SLICE: {STR: %s, LEN: %ld}}\n", i, tk->type, tk->slice.ptr, tk->slice.len);
        }
    }
}

int check_primitive(Token* tk){
    char* prims[10] = {"U0", "U8", "U16", "U32", "U64", "I8", "I16", "I32", "I64", "F64"};

    for(size_t i = 0; i < 10; i++){
        if(strcmp(tk->slice.ptr, prims[i]) == 0){
            return 1;
        }
    }

    return 0;
}

void validate_identifiers(List* token_list){
    for(size_t i = 0; i < token_list->size; i++){
        Token* tk = list_at(token_list, i);

        if(tk->type == TOKEN_TYPE_IDENTIFIER) {
            if(check_primitive(tk)) {
                tk->type = TOKEN_TYPE_PRIMITIVE;
            }
        }
    }
}

List* tokenize(const char* filename){
    char* content = get_file_contents(filename);
    List* token_list = list_new(sizeof(Token), 1);
    
    process_content(token_list, content);
    free(content);

    validate_identifiers(token_list);
    printf("Tokenization Complete: %ld Tokens Generated.\n", token_list->size);

    return token_list;
}