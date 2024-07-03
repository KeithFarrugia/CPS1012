
#include "AdvancedDecode.h"



void validate_and_quote_String(execution_line_t* e){
    int length = strlen(e->string), countQuotes = 0;
    bool exists_alphanumeric= false;
    e->valid = true;
    e->num_of_quot = 0;
    // Count the number of quotation and do the bare minimum validation
    for(int i = 0; i < length; i++){
        
        if(e->string[i] == '\"'){
            //printf("found quotation at %d value : %d\n",i,e->string[i]);
            countQuotes++;
        // check if there is at least 1 non metacharacter or white space
        }else if(e->string[i] == '\\'){
            i++;
        }
        if (isalnum(e->string[i])){exists_alphanumeric= true;}
    }

    // There should only be an even number of quotation marks
    if(!exists_alphanumeric){
        e->valid = false;
        printf("\033[0;31mInput Is Invalid: Must contain at least 1 alphanumeric character.\033[0m\n");
    }
    if(countQuotes %2 != 0){
        printf("\033[0;31mInput Is Invalid: Must contain an even number of quotations.\033[0m\n");
        e->valid = false;
    }
    if(e->string == NULL){
        printf("\033[0;31mInput Is Invalid: Cannot be empty.\033[0m\n");
        e->valid = false;}
}
void displayStats(execution_line_t* e){
    if(e->valid){
        printf("String is valid\n");
    }else{
        printf("String is NOT valid\n");
    }  
}

ErrorType_t Decode_Command(char* string, Command_t* command){
    int length = strlen(string);
    command->pipelines = NULL;
    command->number_of_pipelines = 0;
    PipeLine_t* next = command->pipelines;
    char temp[length+1];
    int i = 0, j=0;
    while(i<length){
        if(string[i] == '\"'){
            temp[j] = string[i]; i++; j++;
            while(i<length){
                if(string[i] == '\\'){
                    temp[j] = string[i]; i++; j++;
                    temp[j] = string[i]; i++; j++;
                }else if(string[i] == '\"'){
                    temp[j] = string[i]; i++; j++;break;
                }else{
                    temp[j] = string[i]; i++; j++;
                }
            }
        }else if(string[i] == '\\'){
            temp[j] = string[i];j++;i++;
            temp[j] = string[i]; i++; j++;
        }
        // Else, the program will copy characters untill it either reaches the end of the string or
        // the next piped program.
        // conditions to malloc : the loop meets a pipe symbol which is not followed by a string
        
        if((string[i] == ';') || i >= length-1){
            if(i == length-1){temp[j] = string[i];j++;} // read character at end of string before 
            i++; temp[j] = '\0';
            if(j == 0){
                return Token_Err;
            }else{
                // First program in Queue
                if(command->pipelines == NULL){
                    next = command->pipelines = (PipeLine_t*)malloc(sizeof(PipeLine_t));
                // Add to Queue
                }else{
                    next->next_pipeline = (PipeLine_t*)malloc(sizeof(PipeLine_t));
                    next = next->next_pipeline;
                }
                next->command = (char*)malloc(sizeof(char*)*strlen(temp)+1);
                command->number_of_pipelines ++;
                next->next_pipeline = NULL;
                strcpy(next->command, temp);
                j=0;
            }
        }else{
            temp[j] = string[i]; i++; j++;
            //printf("temp %s\n",temp);
        }
    }
    return Success;
}

void freeCommandline(Command_t* c){
    PipeLine_t* temp1 = c->pipelines;
    PipeLine_t* temp2;
    while(temp1 != NULL){
        temp2 = temp1;
        temp1 = temp1->next_pipeline;
        free(temp2);
    }
}
void displayCommands(Command_t* command){
    PipeLine_t* temp = command->pipelines;
    int i = 1;
    while(temp != NULL){
        printf("Command %d: \n",i);
        displayPipeline(temp);
        temp = temp->next_pipeline;
        i++;
    }
    printf("Number of pipelines: %d\n",command->number_of_pipelines);
}