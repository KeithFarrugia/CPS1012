#pragma once
#ifndef ENUMS_AND_UTILITY_H
#define ENUMS_AND_UTILITY_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* this enum is used for the input output of the pipe 
 * this is since the read is and write in terms of the integer array are in 
 * opposite order to the visual representation of a pipe and caused some confusion until
 * I got used to the idea
 *     WRITE fd[1] -> [pipe] -> fd[0] READ
 */
typedef enum {
    READ,
    WRITE
}PipePorts_t;

/* Enum of error types
 */
typedef enum{
    Success,
    Fork_Err,
    Wait_Err,
    Exec_Err,
    Malloc_Err,
    Decode_Err,
    EmptyArgs_Err,
    Pipe_Err,
    Set_Directory_Err,
    Token_Err,
    Redirect_Err,
}ErrorType_t;

/* =================================================================================================================
 * Counts the number sub strings a string will need to be split into given a character to be split by
 */
int count_Sub_Strings(char* string, int total_length, char* split_by);
#endif