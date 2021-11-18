/**
 * @file c_printer.c
 * @author Nathan Bourgeois (iridescentrosesfall@gmail.com)
 * @brief Prints a program into C
 * @version 1.0
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
 #include "c_printer.h"
 
List* idx_list;

void emit_header(FILE* fp, const char* filename){
    fprintf(fp, "/* File generated by HolyC Translator! File: %s */\n", filename);
    fprintf(fp, "#include <stdio.h>\n");
}

void emit_preprocessor(FILE* fp, PreProcessor* statement) {
    fprintf(fp, "%s\n", statement->text.ptr);
}

const char* type_to_string(Type type){
    switch(type){
        case TYPE_U0: {
            return "void";
        }
        case TYPE_U8: {
            return "unsigned char";
        }
        case TYPE_U16: {
            return "unsigned short int";
        }
        case TYPE_U32: {
            return "unsigned long int";
        }
        case TYPE_U64: {
            return "unsigned long long int";
        }
        
        case TYPE_I8: {
            return "signed char";
        }
        case TYPE_I16: {
            return "signed short int";
        }
        case TYPE_I32: {
            return "signed long int";
        }
        case TYPE_I64: {
            return "signed long long int";
        }

        case TYPE_F64: {
            return "double";
        }
    }

    return "(null)";
}

void emit_expression(FILE* fp, Expression* statement){
    if(statement->type == EXPRESSION_TYPE_PRINTF){
        fprintf(fp, "printf(%s);\n", statement->buffer);
    } else if (statement->type == EXPRESSION_TYPE_GENERAL) {
        fprintf(fp, "%s;\n", statement->buffer);
    } else if (statement->type == EXPRESSION_TYPE_CALL) {
        fprintf(fp, "%s();\n", statement->buffer);
    }
}

void emit_declaration(FILE* fp, Declaration* statement) {
    fprintf(fp, "%s %s%s %s", statement->externf ? "extern" : "\r", type_to_string(statement->type), statement->pointer ? "*" : "", statement->identifier.ptr);

    if(statement->is_function) {
        fprintf(fp, "(");

        int count = 0;
        while((int)statement->args.types[count] != -1){
            fprintf(fp, "%s %s%s", type_to_string(statement->args.types[count]), statement->args.identifiers[count] ? statement->args.identifiers[count] : "", (int)statement->args.types[count + 1] != -1 ? "," : "");
            count++;
        }

        fprintf(fp, ")");
    }

    fprintf(fp, ";\n");
}

void emit_statement_block(FILE* fp, struct ScopeBlock* block, int tab_count);

void emit_definition(FILE* fp, Definition* statement, int tab_count) {
    fprintf(fp, "%s%s %s", type_to_string(statement->type), statement->pointer ? "*" : "", statement->identifier.ptr);

    if(statement->is_function) {
        fprintf(fp, "(");

        int count = 0;
        while((int)statement->args.types[count] != -1){
            fprintf(fp, "%s%s %s%s", type_to_string(statement->args.types[count]), statement->args.pointer[count] ? "*" : "", statement->args.identifiers[count] ? statement->args.identifiers[count] : "", (int)statement->args.types[count + 1] != -1 ? "," : "");
            count++;
        }

        fprintf(fp, ")");
    }

    fprintf(fp, "{\n");
    emit_statement_block(fp, statement->function_content, tab_count + 1);
    fprintf(fp, "}\n");
}

void emit_statement_block(FILE* fp, struct ScopeBlock* block, int tab_count){

    for(int i = 0; i < tab_count; i++){
        fprintf(fp, "\t");
    }
    for(size_t i = 0; i < block->statement_list->size; i++){
        Statement* statement = list_at(block->statement_list, i);

        switch (statement->type) {
            case STATEMENT_TYPE_DECLARATION: {
                emit_declaration(fp, (Declaration*)statement->statementData);
                break;
            }
            
            case STATEMENT_TYPE_DEFINITION: {
                emit_definition(fp, (Definition*)statement->statementData, tab_count);
                break;
            }
            
            case STATEMENT_TYPE_EXPRESSION: {
                if(block->parent == NULL){
                    list_push(idx_list, &i);
                } else {
                    emit_expression(fp, (Expression*)statement->statementData);
                }
                break;
            }

            case STATEMENT_TYPE_PREPROCESSOR: {
                emit_preprocessor(fp, (PreProcessor*)statement->statementData);
                break;
            }
        
            default: {
                CHECK_FAILED("INVALID STATEMENT TYPE: %d", statement->type);
                break;
            }
        }
    }
}

void emit_main(FILE* fp, struct ScopeBlock* block){

    if(idx_list->size != 0){
        fprintf(fp, "int main(int argc, char** argv){\n");

        for(size_t i = 0; i < idx_list->size; i++){
            int idx = *((int*)list_at(idx_list, i));

            Statement* statement = (Statement*)list_at(block->statement_list, idx);

            fprintf(fp, "\t");
            emit_expression(fp, (Expression*)statement->statementData);
        }

        fprintf(fp, "\treturn 0;\n}\n");
    }
}


void emit_c(Program* program, const char* filename){
    FILE* fp = fopen(filename, "w");
    CHECK_NOT_NULL(fp, "Could not access output file!");

    idx_list = list_new(sizeof(int), 32);

    emit_header(fp, filename);
    emit_statement_block(fp, &program->block, 0);

    emit_main(fp, &program->block);
    list_delete(idx_list);

    fclose(fp);

    printf("Done!\n\n");
}