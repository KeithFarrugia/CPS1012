#pragma once
#ifndef INPUT_OUTPUT_REDIRECTION_H
#define INPUT_OUTPUT_REDIRECTION_H

#include "EnumsAndUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*=====================================================================================================
 * Function checks whether a given file exists by opening the file in read mode
 */
bool file_exists(const char *f);

/*=====================================================================================================
 * Code Concept taken from lecture videos
 * Program opens new file, sets flags and mode, replaces current file with the path
 * fd : file descriptor, to be changed
 * path: name of path, new output/ inptu path
 * flags: flags
 * mode: file open mode (read only, write only etc...)
 */
ErrorType_t reopen(int fd, char *path, int flags, mode_t mode);
/*=====================================================================================================
 * Code Concept taken from lecture videos
 * input: file for which input should be set to
 */
ErrorType_t redirect_input(char *input);
/*=====================================================================================================
 * Code Concept taken from lecture videos
 * output: file for which output should be set to
 * trunc : Whether output file should be appended or truncated
 */
ErrorType_t redirect_output(char *output, bool trunc);
#endif