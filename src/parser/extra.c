int _xarstnrastk = 0;
/**
 * @brief Checks if an identifier is a function
 * 
 * @param block Scoped Block to check from
 * @param identifier Identifier to look for
 * @return int 0 if cannot be found, 1 if could find and is a function
 *
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
*/
