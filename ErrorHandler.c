#include "ErrorHandler.h"

void handleErr(ErrorType_t e){
    switch (e){
    case Success:
        break;
    case Fork_Err:
        printf("\033[0;31mError: Fork Failed\033[0m\n");
        break;
    case Wait_Err:
        printf("\033[0;31mError: Wait Failed\033[0m\n");
        break;
    case Exec_Err:
        printf("\033[0;31mError: Execution Failed\033[0m\n");
        break;
    case Malloc_Err:
        printf("\033[0;31mError: Malloc Failed\033[0m\n");
        break;
    case Decode_Err:
        printf("\033[0;31mError: Docoding Failed\n \033[0;37m");
        break;
    case EmptyArgs_Err:
        printf("\033[0;31mError: No Arguements were passed\033[0m\n");
        break;
    case Pipe_Err:
        printf("\033[0;31mError: Pipe Failed\033[0m\n");
        break;
    case Set_Directory_Err:
        printf("\033[0;31mError: Failed to set directory\033[0m\n");
        break;
    case Token_Err:
        printf("\033[0;31mError: Failed to tokenise input\033[0m\n");
        break;
    case Redirect_Err:
        printf("\033[0;31mError: Failed to redirect input/output file\033[0m\n");
        break;
    default:
        break;
    }
}