#include "ExecuteFunctions.h"
#include "Builtin.h"
#include "AdvancedDecode.h"
#include "ErrorHandler.h"
#include "InputOutputRedirect.h"
#include "DecodePipesAndArgs.h"
#include "linenoise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX_ARGS 16

// Must be greater than 2
#define MAX_NAME 32

// Exit code to use when execvp fails
#define CUSTOM_EXIT_CODE 4
int main(int argc, char * argv[], char* envp[]) {
    char* Shellname = "Squirtle>$ "; //"\033[0;36mSquirtle>$ \033[0;37";
    char** string_of_args;
    execution_line_t e;
    Command_t commandline;
    PipeLine_t* currentPipeline;
    char* inputline;
    ErrorType_t err;
    //==============================================================
    while((inputline = linenoise(Shellname)) != NULL) {
        // DECODE COMMAND
        Decode_Command(inputline,&commandline);
        
        currentPipeline = commandline.pipelines;
        while(currentPipeline != NULL){
            e.string = currentPipeline->command;
            validate_and_quote_String(&e);
            if(e.valid){
                if(validateToken(currentPipeline->command) == Success){
                    // DECODING PIPELINE
                    Decode_Redirect_in(currentPipeline->command, currentPipeline);
                    Decode_Redirect_out(currentPipeline->command, currentPipeline);
                    err = split_PipeLine_into_Programs(currentPipeline->command, currentPipeline);

                    // ERROR CHECKING
                    if(err != Success){handleErr(err);freePipeline(currentPipeline); break;}
                    if(file_exists(currentPipeline->file_in) == true){
                        string_of_args = Queue_to_String(currentPipeline->args);
                        if(compare_builtin(string_of_args) == true){
                            
                        }else{
                            handleErr(execute_pipeline(currentPipeline, false));
                        }
                        free(string_of_args);
                    }else{printf("\033[0;31mFiles where not found in directory\033[0m\n");}
                    
                }
            }else{}
            currentPipeline = currentPipeline->next_pipeline;
        }
        freeCommandline(&commandline);
        linenoiseFree(inputline);
    }
    return EXIT_SUCCESS;
}