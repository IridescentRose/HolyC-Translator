#pragma once

/**
 * @brief All Holy C types
 * 
 */
typedef enum{
    TYPE_U0,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_I0,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_F64,
} Type;

typedef struct{
    Type primitive;
    char is_pointer;
    char is_extern;
    char is_const;
    char is_static;
    char is_volatile;
    char is_register;
} CType;

/**
 * @brief Arguments object, contains arguments to a function as types, identifiers, and if certain types are pointers
 * 
 */
typedef struct{
    CType types[16];
    char* identifiers[16];
} Arguments;
