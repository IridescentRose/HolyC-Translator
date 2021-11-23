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