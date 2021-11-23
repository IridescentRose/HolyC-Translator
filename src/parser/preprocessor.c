#include "preprocessor.h"

/**
 * @brief Given the location of a command - executes a command and inserts it into an #exe{}
 * 
 * @param res 
 * @param token 
 */
void execute_exe(char* res, Token* token){
    int len = 0;

    while( (res+6)[len] != '}'){
        len++;
    }
    //Replace with 0
    (res+6)[len] = 0;

    char* command = make_slice(res, 6, len);

    FILE* fp = popen(command, "r");
    
    char buffer[1024];
    char* res2 = fgets(buffer, sizeof(buffer), fp);
    
    CHECK_NOT_NULL(res2, "Error: Couldn't execute process! Command: %s\n", command);
    
    token->slice.ptr = realloc(token->slice.ptr, token->slice.len - len - 6 + strlen(res2));
    
    res[0] = 0;
    res2[strlen(res2) - 1] = 0;

    strcat(token->slice.ptr, "\"");
    strcat(token->slice.ptr, res2);
    strcat(token->slice.ptr, "\"");

    free(command);
}

/**
 * @brief Validates preprocessor instructions to remove AOT and JIT specifics and run EXE commands
 * 
 * @param token Preprocessor token to validate
 */
void validate_preprocessor(Token* token) {
    char* res1 = strstr(token->slice.ptr, "#ifaot");
    char* res2 = strstr(token->slice.ptr, "#ifjit");

    if(res1 || res2){
        free(token->slice.ptr);
        token->slice.ptr = dupe_string("#if 0");
        token->slice.len = 6;
    }

    char* res3 = strstr(token->slice.ptr, "#exe{");

    if(res3)
        execute_exe(res3, token);
}


void make_preprocessor(List* list, Token* token, size_t* idx) {
    validate_preprocessor(token);
    
    Statement statement;
    statement.type = STATEMENT_TYPE_PREPROCESSOR;

    PreProcessor* preproc = (PreProcessor*)calloc(1, sizeof(PreProcessor));
    preproc->text = token->slice;

    statement.statementData = preproc;
    list_push(list, &statement);

    (*idx)++;
}