#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "translate_utils.h"

/* A helper function used in translate.c */
void write_inst_string(FILE* output, const char* name, char** args, int num_args) {
    fprintf(output, "%s", name);
    for (int i = 0; i < num_args; i++) {
        fprintf(output, " %s", args[i]);
    }
    fprintf(output, "\n");
}

/* A helper function used in translate.c */
void write_inst_hex(FILE *output, uint32_t instruction) {
    fprintf(output, "%08x\n", instruction);
}

/* A helper function used in assembler.c */
int is_valid_label(const char* str) {
    if (!str) {
        return 0;
    }

    int first = 1;
    while (*str) {
        if (first) {
            if (!isalpha((int) *str) && *str != '_') {
                return 0;   // does not start with letter or underscore
            } else {
                first = 0;
            }
        } else if (!isalnum((int) *str) && *str != '_') {
            return 0;       // subsequent characters not alphanumeric
        }
        str++;
    }
    return first ? 0 : 1;   // empty string is invalid
}

/* Translate the input string into a signed number. The number is then 
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND. 

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take 
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants. 

   You should store the result into the location that OUTPUT points to. The 
   function returns 0 if the conversion proceeded without errors, or -1 if an 
   error occurred.
 */
int translate_num(long int* output, const char* str, long int lower_bound,    
    long int upper_bound) {
 //   printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@TRYING TO TRANSLATE%s\n", str);
    if (!str || !output) {
        return -1;
    }
    char* end;
    long int sig_num = strtol(str, &end, 0);
    
    if (sig_num < lower_bound || sig_num > upper_bound) {
        return -1;
    }
 //   printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ %ld\n", sig_num);
    *output = sig_num;
    return 0;
}

/* Translates the register name to the corresponding register number. Please
   see the MIPS Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char* str) {
    if (strcmp(str, "$zero") == 0)      return 0;
    else if (strcmp(str, "$0") == 0)    return 0;
    else if (strcmp(str, "$at") == 0)   return 1;
    else if (strcmp(str, "$v0") == 0)   return 2;
    else if (strcmp(str, "$a0") == 0)   return 4;
    else if (strcmp(str, "$a1") == 0)   return 5;
    else if (strcmp(str, "$a2") == 0)   return 6;
    else if (strcmp(str, "$a3") == 0)   return 7;
    else if (strcmp(str, "$t0") == 0)   return 8;
    else if (strcmp(str, "$t1") == 0)   return 9;
    else if (strcmp(str, "$t2") == 0)   return 10;
    else if (strcmp(str, "$t3") == 0)   return 11;
    else if (strcmp(str, "$s0") == 0)   return 16;
    else if (strcmp(str, "$s1") == 0)   return 17;
    else if (strcmp(str, "$s2") == 0)   return 18;
    else if (strcmp(str, "$s3") == 0)   return 19;
    else if (strcmp(str, "$sp") == 0)   return 29;
    else if (strcmp(str, "$fp") == 0)   return 30;
    else if (strcmp(str, "$ra") == 0)   return 31;
    else                                return -1;
}
