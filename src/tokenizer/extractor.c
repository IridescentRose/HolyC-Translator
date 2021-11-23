/**
 * @file extractor.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Token extractor
 * @version 1.0
 * @date 2021-11-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "extractor.h"

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
 * @brief Checks keyword against list of primitives
 * 
 * @param tk Token to be checked
 * @return int 0 if not found, 1 if found
 */
int check_keyword(Token* tk){
    char* prims[20] = {"switch", "case", "default", "break", "goto", "const", "static", "volatile", "register", "if", "else", "enum", "class", "union", "extern", "return", "do", "while", "for", "sizeof"};

    for(size_t i = 0; i < 20; i++){
        if(strcmp(tk->slice.ptr, prims[i]) == 0){
            tk->keyword = (KeywordType)(i + 1);
            return 1;
        }
    }

    return 0;
}

void extract_token_single_char(char* content, size_t* idx, TokenType type, int line, int* cur, List* token_list){
    Token tk;
    tk.slice.ptr = make_slice(content, (*idx)++, 1);
    tk.slice.len = 1;
    tk.type = type;
    tk.line = line;
    tk.cursor = (*cur)++;

    list_push(token_list, &tk);
}

void extract_token_preprocessor(char* content, size_t* idx, int line, int* cur, List* token_list){
    Token tk;
    tk.slice.len = 0;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_PREPROCESSOR;
    size_t len = strlen(content);

    for(;content[tk.slice.len + *idx] != '\n' && (tk.slice.len + *idx) < len; tk.slice.len++){}

    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    if(strstr(tk.slice.ptr, "#help_file") || strstr(tk.slice.ptr, "#help_index")){
        free(tk.slice.ptr);
        tk.slice.ptr = NULL;
    }

    tk.line = line;
    tk.cursor = (*cur);

    (*idx) += tk.slice.len;
    (*cur) += tk.slice.len;

    if(tk.slice.ptr){
        list_push(token_list, &tk);
    }
}

Token extract_token_identifier(char* content, size_t* idx, int line, int* cur){
    Token tk;
    tk.slice.len = 0;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_IDENTIFIER;

    while(1){
        char s = content[tk.slice.len + *idx];

        if(!((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9') || s == '_')){
            break;
        }else{
            tk.slice.len++;
        }
    }
    
    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    if(check_primitive(&tk))
        tk.type = TOKEN_TYPE_PRIMITIVE;

    if(check_keyword(&tk))
        tk.type = TOKEN_TYPE_KEYWORD;

    tk.line = line;
    tk.cursor = (*cur);

    (*idx) += tk.slice.len;
    (*cur) += tk.slice.len;
    
    return tk;
}

Token extract_token_literal(char* content, size_t* idx, int line, int* cur){
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

    tk.line = line;
    tk.cursor = (*cur);

    (*idx) += tk.slice.len;
    (*cur) += tk.slice.len;
    
    return tk;
}

void extract_token_string(char* content, size_t* idx, int line, int* cur, List* token_list){
    Token tk;
    tk.slice.len = 1;
    tk.slice.ptr = 0;
    tk.type = TOKEN_TYPE_STRING;

    for(;content[tk.slice.len + *idx] != '\"'; tk.slice.len++){}
    tk.slice.len++;

    tk.slice.ptr = make_slice(content, *idx, tk.slice.len);

    
    tk.line = line;
    tk.cursor = (*cur);

    (*idx) += tk.slice.len;
    (*cur) += tk.slice.len;
    
    list_push(token_list, &tk);
}
