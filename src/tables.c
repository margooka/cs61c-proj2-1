   #include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

#define INITIAL_SIZE 5
#define SCALING_FACTOR 2

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    SymbolTable * s = (SymbolTable*) malloc(sizeof(SymbolTable));
    if (!s) {
        allocation_failed();
    }
    s->tbl = (Symbol*) malloc(INITIAL_SIZE*sizeof(Symbol));
    if (!s->tbl) {
        free(s);
        allocation_failed();
    }
    s->len = 0;
    s->cap = INITIAL_SIZE;
    s->mode = mode;
    return s;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {

    if (!table) {
        return;
    }
    for(int i = 0; i<table->len; i++) { 
        if (&(table->tbl[i])) {
            free(table->tbl[i].name); 
        }     
    }
    free(table->tbl);
    free(table);
}

/* A suggested helper function for copying the contents of a string. */
static char* create_copy_of_str(const char* str) {
    size_t len = strlen(str) + 1;
    char *buf = (char *) malloc(len);
    if (!buf) {
        free(buf);
        allocation_failed();
    }
    strncpy(buf, str, len); 
    return buf;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added. 

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    if (!table || !name || !table->tbl) {
        return -1;
    }
    if (addr % 4) {
        addr_alignment_incorrect();
        return -1;
    }
    if (table->mode == SYMTBL_UNIQUE_NAME) {
        for(int i = 0; i<table->len; i++) {
            if (strcmp(table->tbl[i].name, name) == 0) {
                name_already_exists(name);
                return -1;
            }
        }
    }
    if (table->len == table->cap) {
        table->tbl = realloc(table->tbl, table->len*SCALING_FACTOR*sizeof(Symbol)); 
        if (!table->tbl) {
            allocation_failed();
        }
        table->cap *= SCALING_FACTOR;
    }
    Symbol * mapping = &(table->tbl[table->len]);
    mapping->name = create_copy_of_str(name);
    mapping->addr = addr;
    table->tbl[table->len] = *mapping;
    table->len += 1;
    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    if (!table) {
        return -1;
    }
    for(int i = 0; i<table->len; i++) {
        if (strcmp(table->tbl[i].name, name) == 0) {
            return table->tbl[i].addr;
        }
    }
    return -1;   
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    for(int i = 0; i<table->len; i++) {
        write_symbol(output, table->tbl[i].addr, table->tbl[i].name);
    }  
}
