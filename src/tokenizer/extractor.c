#include "extractor.h"

Token extract_token_preprocessor(char* content, size_t* idx){
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

    *idx += tk.slice.len;
    return tk;
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

Token extract_token_identifier(char* content, size_t* idx){
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