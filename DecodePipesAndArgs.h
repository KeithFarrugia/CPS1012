#pragma once
#ifndef DECODE_PIPES_AND_ARGS_H
#define DECODE_PIPES_AND_ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "EnumsAndUtility.h"

// #define MAX_ARGS_SIZE 1024
// #define MAX_ARGS_NUM 10
//
typedef struct single_arg{
    char* s_arg;
    struct single_arg* next_arg;
}Single_arg_t;

typedef struct args{
    Single_arg_t* single_argument;
    struct args* next;
    int number_of_args;
}Arguments_t;

typedef struct pipeline{
    Arguments_t* args;
    char* file_in;
    char* file_out;
    bool truncate;
    int number_of_pipes;
    int number_of_programs;
    struct pipeline* next_pipeline;
    char* command;
}PipeLine_t;

/* =================================================================================================================
 * Function checks whether any tokens specified in the tokens[] in this case |, <, > are present at the start or
 * end of a string. This is in order to validate as specified in task 3 a.
 */
ErrorType_t validateToken(char* string);
/* =================================================================================================================
 * The function takes in a string and a pipline struct. 
 * Its purpose is to detect whether the user specified a file to redirect the output to.
 * How the function works is that it reads the string up untill it finds the '>' character. It also takes into account >>.
 * After which it then reads the file in two ways; either including spaces if the user bound it between "..."
 * or just reading until a space is found if he didn't. While reading it replaces any character with spaces,
 * this is so the same string can later be passed into the split by pipes and args functions.
 * After reading the string it mallocs it into the file_in variable of pipline.
 */
ErrorType_t Decode_Redirect_out(char* string, PipeLine_t* pipeline);
/* =================================================================================================================
 * The function takes in a string and a pipline struct. 
 * Its purpose is to detect whether the user specified a file to redirect the input to.
 * How the function works is that it reads the string up untill it finds the '<' character.
 * After which it then reads the file in two ways; either including spaces if the user bound it between "..."
 * or just reading until a space is found if he didn't. While reading it replaces any character with spaces,
 * this is so the same string can later be passed into the split by pipes and args functions.
 * After reading the string it mallocs it into the file_in variable of pipline.
 */
ErrorType_t Decode_Redirect_in(char* string, PipeLine_t* pipeline);

/* =================================================================================================================
 * This function splits a program into a number of arguments for example
 * "echo hello" => 
 * args ->  "echo", "hello"
 * number_of_args = 2
 */
ErrorType_t split_Program_into_Arguments(char* program, Arguments_t* argument_list);


char** Queue_to_String(Arguments_t* args);

void freeArguments(Arguments_t* args);

void displayArgList(Arguments_t* a);

/* =================================================================================================================
 * This function splits a pipline of programs into seperate strings 
 * and uses the previous function to split each program into seperate arguments
 * "echo hello | figlet" => 
 * "echo hello"  "figlet"
 * number_of_pipes = 1
 * number_of_programs = 2
 */
ErrorType_t split_PipeLine_into_Programs(char* command, PipeLine_t* pipeline);
void freePipeline(PipeLine_t* pipeline);
void displayPipeline(PipeLine_t* pipeline);

#endif