#pragma once
#ifndef EXECUTE_FUNCTIONS_H
#define EXECUTE_FUNCTIONS_H
#include "InputOutputRedirect.h"
#include "DecodePipesAndArgs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "EnumsAndUtility.h"

/* ===================================================================================================
 * Takes as input a pid and waits for that pid to finish
 * If any errors occure the respective enum is returned
 */
ErrorType_t wait_for_child(pid_t pid);

/* ===================================================================================================
 * TASK 1A
 * Program executes a given argument list using the fork function
 * [pid_child] : Takes pid pointer to fill with child pid 
 */
ErrorType_t fork_exec(char *const *args, pid_t* pid_child);


/* ===================================================================================================
 * TASK 1 B
 * Execute a program given two pipes and a pid pointer to return the child pid.
 * TASK 1 E
 * Function was Modified in order to be able to allow input or output redirection
 * Function Parameters:
 * [program] will hold the program to be run,
 * [pid_child] sets the pid of the child for any wait functions 
 * [pipe_fd_in] is the left pipe
 * [pipe_fd_out] is the right pipe
 * [file_in] is the redirect input
 * [file_out] is the redirect output
 * [truncate] true : output file is truncated | false : output file is appended to
*/
ErrorType_t fork_exec_pipe(char ** program, pid_t* pid_child, 
                           int pipe_fd_in[2], int pipe_fd_out[2],
                           char* file_in, char* file_out, bool truncate);


/* ===================================================================================================
 * TASK 1 C
 * Function executes a given pipline by using the function "fork_exec_pipe"
 * TASK 1 D
 * Funcion was modified in order to allow waiting for the last program in pipeline
 * TASK 1 E
 * Function was further modified to allow input and output redirection
 * 
*/
ErrorType_t execute_pipeline(PipeLine_t* pipeline, bool async);

#endif