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

Token *get_next(List *token_list, size_t *idx)
{
    (*idx)++;
    Token *next_tok = list_at(token_list, *idx);
    CHECK_NOT_NULL(next_tok, "Error: Unexpected end of file!");

    return next_tok;
}

/**
 * @brief Get the type object
 * 
 * @param slice Slice for a type ID
 * @return Type Type
 */
Type get_primitive(StringSlice slice)
{

    if (strcmp(slice.ptr, "U0") == 0)
    {
        return TYPE_U0;
    }
    if (strcmp(slice.ptr, "U8") == 0)
    {
        return TYPE_U8;
    }
    if (strcmp(slice.ptr, "U16") == 0)
    {
        return TYPE_U16;
    }
    if (strcmp(slice.ptr, "U32") == 0)
    {
        return TYPE_U32;
    }
    if (strcmp(slice.ptr, "U64") == 0)
    {
        return TYPE_U64;
    }
    if (strcmp(slice.ptr, "I0") == 0)
    {
        return TYPE_I0;
    }
    if (strcmp(slice.ptr, "I8") == 0)
    {
        return TYPE_I8;
    }
    if (strcmp(slice.ptr, "I16") == 0)
    {
        return TYPE_I16;
    }
    if (strcmp(slice.ptr, "I32") == 0)
    {
        return TYPE_I32;
    }
    if (strcmp(slice.ptr, "I64") == 0)
    {
        return TYPE_I64;
    }
    if (strcmp(slice.ptr, "F64") == 0)
    {
        return TYPE_F64;
    }

    return -1;
}

/**
 * @brief Get the primitive identifier object
 * 
 * @param token_list List of tokens
 * @param token Initial token
 * @param idx Index of current token
 * @param type Type to write to
 * @param identifier Identifier to write to
 */
void get_primitive_identifier(List *token_list, Token *token, size_t *idx, CType *type, StringSlice *identifier)
{
    memset(type, 0, sizeof(CType));
    type->primitive = get_primitive(token->slice);

    Token *next_tok = get_next(token_list, idx);

    if (next_tok->slice.ptr[0] == '*')
    {
        type->is_pointer = 1;
        next_tok = get_next(token_list, idx);
    }

    if (next_tok->type != TOKEN_TYPE_IDENTIFIER)
    {
        CHECK_FAILED("Error: Expected identifier after primitive declaration! Found %s at %d:%d!\n", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
    }
    (*identifier) = next_tok->slice;
}

/**
 * @brief Get the primitive identifier object
 * 
 * @param token_list List of tokens
 * @param token Initial token
 * @param idx Index of current token
 * @param type Type to write to
 * @param identifier Identifier to write to
 */
void get_primitive_identifier_argument(List *token_list, Token *token, size_t *idx, CType *type, char **identifier)
{
    memset(type, 0, sizeof(CType));

    if (token->type == TOKEN_TYPE_KEYWORD)
    {
        //Can only really be const or volatile.
        if (token->keyword == KEYWORD_TYPE_CONST)
        {
            type->is_const = 1;
        }
        else if (token->keyword == KEYWORD_TYPE_VOLATILE)
        {
            type->is_volatile = 1;
        }
        else
        {
            CHECK_FAILED("Error: Expected valid keyword for argument parameter! Found %s at %d:%d!\n", token->slice.ptr, token->line, token->cursor);
        }

        token = get_next(token_list, idx);
    }

    type->primitive = get_primitive(token->slice);

    Token *next_tok = get_next(token_list, idx);

    if (next_tok->slice.ptr[0] == '*')
    {
        type->is_pointer = 1;
        next_tok = get_next(token_list, idx);
    }

    if (next_tok->type != TOKEN_TYPE_IDENTIFIER)
    {
        (*identifier) = NULL;

        (*idx)--; //Reverse one, just in case this is the end
    }
    else
    {
        (*identifier) = next_tok->slice.ptr;
    }
}

/**
 * @brief Get the arguments object
 * 
 * @param token_list Token List to read
 * @param idx Index of current token
 * @param args Arguments to write to
 */
void get_arguments(List *token_list, size_t *idx, Arguments *args)
{
    memset(args, 0, sizeof(Arguments));
    memset(args->types, -1, sizeof(args->types));

    Token *next_tok = get_next(token_list, idx);

    int count = 0;

    //While we don't have ')'
    while (next_tok->slice.ptr[0] != ')' && count < 16)
    {

        if (next_tok->slice.ptr[0] == ',')
        {
            next_tok = get_next(token_list, idx);
        }

        get_primitive_identifier_argument(token_list, next_tok, idx, &args->types[count], &args->identifiers[count]);
        count++;

        next_tok = get_next(token_list, idx);
    }

    //We exit, knowing that the token is ')', and we will read the next
}

void parse_token_program(struct ScopeBlock *block, List *token_list, size_t *idx);

/**
 * @brief Determines a primitive given a context
 * 
 * @param token_list Token list
 * @param token Token
 * @param idx Index
 * @param block Block
 */
void determine_primitive(List* token_list, Token* token, size_t* idx, struct ScopeBlock* block, CType type_q) {
    CType type;
    StringSlice identifier;

    get_primitive_identifier(token_list, token, idx, &type, &identifier);

    type.is_const |= type_q.is_const;
    type.is_extern |= type_q.is_extern;
    type.is_register |= type_q.is_register;
    type.is_static |= type_q.is_static;
    type.is_volatile |= type_q.is_volatile;

    Token *next_tok = get_next(token_list, idx);

    if (next_tok->type == TOKEN_TYPE_TERMINATOR) {
        make_variable_declaration(block->statement_list, identifier, type);
        (*idx)++;
    } else if (next_tok->type == TOKEN_TYPE_ASSIGNMENT) {
        if(!type.is_const){
            make_variable_declaration(block->statement_list, identifier, type);
            make_expression_compound_assign(token_list, identifier, idx, block->statement_list);
        } else {
            make_variable_declaration_assign(token_list, idx, identifier, type, block->statement_list);
        }
    } else if (next_tok->type == TOKEN_TYPE_PUNCTUATOR) {
        //Grab args 'til next punctuator - check terminator or scoping
        Arguments args;
        get_arguments(token_list, idx, &args);

        next_tok = get_next(token_list, idx);

        if (next_tok->type == TOKEN_TYPE_TERMINATOR) {
            make_function_declaration(block->statement_list, identifier, type, args);
            (*idx)++;
        } else if (next_tok->type == TOKEN_TYPE_SCOPING) {
            struct ScopeBlock *statementBlock = (struct ScopeBlock *)calloc(1, sizeof(struct ScopeBlock));
            statementBlock->parent = block;
            statementBlock->statement_list = list_new(sizeof(Statement), 32);

            (*idx)++;
            Token *test = list_at(token_list, *idx);

            while (test && test->type != TOKEN_TYPE_SCOPING) {
                parse_token_program(statementBlock, token_list, idx);
                test = list_at(token_list, *idx);
            }

            make_function_definition(block->statement_list, identifier, type, args, statementBlock);
            (*idx)++;
        } else {
            CHECK_FAILED("Error: Unexpected Token %s found at %d:%d\n", token->slice.ptr, token->line, token->cursor);
        }
    } else {
        CHECK_FAILED("Error: Unexpected Token %s found at %d:%d\n", token->slice.ptr, token->line, token->cursor);
    }
}

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

void check_qualifiers(List* token_list, Token* token, size_t* idx, CType* type_q) {

    switch (token->keyword) {
        case KEYWORD_TYPE_CONST:{
            type_q->is_const = 1;
            break;
        }
        case KEYWORD_TYPE_EXTERN:{
            type_q->is_extern = 1;
            break;
        }
        case KEYWORD_TYPE_REGISTER:{
            type_q->is_register = 1;
            break;
        }
        case KEYWORD_TYPE_STATIC:{
            type_q->is_static = 1;
            break;
        }
        case KEYWORD_TYPE_VOLATILE:{
            type_q->is_volatile = 1;
            break;
        }
        default:{
            CHECK_FAILED("Error: Expected Qualifier! Found %s at %d:%d!", token->slice.ptr, token->line, token->cursor);
            break;
        }
    }

    Token* next_tok = get_next(token_list, idx);

    if(next_tok->type == TOKEN_TYPE_KEYWORD) {
        check_qualifiers(token_list, token, idx, type_q);
    } else {
        (*idx)--;
    }
}

void parse_token_program(struct ScopeBlock *block, List *token_list, size_t *idx)
{
    Token *token = list_at(token_list, *idx);
    CType type_q;
    memset(&type_q, 0, sizeof(CType));

    switch (token->type)
    {
    case TOKEN_TYPE_PREPROCESSOR:
    {
        make_preprocessor(block->statement_list, token, idx);
        break;
    }

    case TOKEN_TYPE_SCOPING:
    {
        //TODO: Support arbitrary scoped blocks
        break;
    }

    case TOKEN_TYPE_IDENTIFIER:
    {
        if(check_identifier_function(block, token->slice)){
            Token* next_tok = get_next(token_list, idx);
            if(next_tok->type == TOKEN_TYPE_TERMINATOR){
                make_expression_call(token->slice, block->statement_list);
                (*idx)++;
            } else {
                (*idx)--;
                make_expression_general(token_list, token->slice, idx, block->statement_list, NULL);
            }
        } else {
            make_expression_general(token_list, token->slice, idx, block->statement_list, NULL);
        }
        break;
    }

    case TOKEN_TYPE_STRING:
    {
        make_expression_printf(token_list, token, idx, block->statement_list);
        break;
    }

    case TOKEN_TYPE_PRIMITIVE:
    {
        determine_primitive(token_list, token, idx, block, type_q);
        break;
    }

    case TOKEN_TYPE_KEYWORD:
    {
        switch(token->keyword) {

            case KEYWORD_TYPE_CONST:
            case KEYWORD_TYPE_EXTERN:
            case KEYWORD_TYPE_REGISTER:
            case KEYWORD_TYPE_STATIC:
            case KEYWORD_TYPE_VOLATILE: {
                check_qualifiers(token_list, token, idx, &type_q);
                token = get_next(token_list, idx);
                determine_primitive(token_list, token, idx, block, type_q);
                break;
            }

            case KEYWORD_TYPE_IF: {
                make_conditional_if(token_list, idx, block->statement_list, block, 0);
                break;
            }

            case KEYWORD_TYPE_ELSE: {
                Token* next_tok = get_next(token_list, idx);

                if (next_tok->type == TOKEN_TYPE_KEYWORD && next_tok->keyword == KEYWORD_TYPE_IF) {
                    make_conditional_if(token_list, idx, block->statement_list, block, 1);
                } else if (next_tok->type == TOKEN_TYPE_SCOPING) {
                    (*idx)--;
                    make_conditional_else(token_list, idx, block->statement_list, block);
                } else {
                    CHECK_FAILED("Error: Expected valid token after keyword else! Found %s at %d:%d", next_tok->slice.ptr, next_tok->line, next_tok->cursor);
                }

                break;
            }

            case KEYWORD_TYPE_RETURN: {
                make_expression_general(token_list, token->slice, idx, block->statement_list, " ");
                break;
            }

            default: {
                CHECK_FAILED("KEYWORD DETECTED!\n");
                break;
            }
        }
        break;
    }

    default:
    {
        CHECK_FAILED("Error: Unknown Token %d %s found at %d:%d\n", token->type, token->slice.ptr, token->line, token->cursor);
        break;
    }
    }
}

Program *parse(List *token_list)
{
    Program *program = calloc(1, sizeof(Program));
    program->block.statement_list = list_new(sizeof(Statement), 32);

    size_t idx = 0;
    while (idx < token_list->size)
    {
        parse_token_program(&program->block, token_list, &idx);
    }

    printf("Parser: AST Generated!\nParser: Main scope block contains %ld statements!\n", program->block.statement_list->size);
    return program;
}

void free_program(struct ScopeBlock *block)
{

    for (size_t i = 0; i < block->statement_list->size; i++)
    {
        Statement *st = list_at(block->statement_list, i);

        switch (st->type)
        {
        case STATEMENT_TYPE_PREPROCESSOR:
        {
            PreProcessor *preproc = (PreProcessor *)st->statementData;
            free(preproc);
            break;
        }

        case STATEMENT_TYPE_DECLARATION:
        {
            Declaration *decl = (Declaration *)st->statementData;
            if(decl->expr){
                free(decl->expr);
            }
            free(decl);
            break;
        }

        case STATEMENT_TYPE_DEFINITION:
        {
            Definition *def = (Definition *)st->statementData;
            free_program(def->function_content);
            free(def->function_content);
            free(def);
            break;
        }

        case STATEMENT_TYPE_EXPRESSION:
        {
            Expression *expr = (Expression *)st->statementData;
            free(expr);
            break;
        }

        default:
        {
            free(st->statementData);
            break;
        }
        }
    }

    list_delete(block->statement_list);
}
