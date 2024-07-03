#pragma once
#ifndef ADVANCED_DECODE_H
#define ADVANCED_DECODE_H

#include "DecodePipesAndArgs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct{
    char* string;
    int num_of_quot;
    bool valid;
}execution_line_t;

typedef struct command{
    PipeLine_t* pipelines;
    int number_of_pipelines;
}Command_t;

/* ===========================================================================
 * function is used to make some basic validation such as checking that there:
 * exists at least one non symbolic character
 * there are an even number of quotes; 
 */
void validate_and_quote_String(execution_line_t* e);

void displayStats(execution_line_t* e);

ErrorType_t Decode_Command(char* string, Command_t* command);

void freeCommandline(Command_t* c);
void displayCommands(Command_t* command);
#endif