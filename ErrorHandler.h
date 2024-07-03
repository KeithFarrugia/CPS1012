#pragma once
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "EnumsAndUtility.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Function takes a given Error enum and OUtputs the respective message
 */
void handleErr(ErrorType_t e);
#endif