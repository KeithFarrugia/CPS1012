#pragma once
#ifndef BUILTIN_H
#define BUILTIN_H

#include "ErrorHandler.h"
#include "EnumsAndUtility.h"
#include "linenoise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/* =================================================================================
 * Custom Bultin commands structs and function type
*/
typedef ErrorType_t (* builtin_t )( char **) ;
typedef struct {
    char* name;
    builtin_t method; 
}builtin_command_t;

/* =================================================================================
 * Custom Bultin Functions
*/
ErrorType_t buitin_exit ( char ** args );
ErrorType_t buitin_cd ( char ** args );
ErrorType_t buitin_cwd ( char ** args );
ErrorType_t buitin_ver  ( char ** args );



/* =================================================================================
 * Decode a given function to check whether it contains a custom command
 * returns true if a built in command matching that of the input string is found
 */
bool compare_builtin(char** string);
#endif