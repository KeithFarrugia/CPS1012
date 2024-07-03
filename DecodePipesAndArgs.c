#include "DecodePipesAndArgs.h"



ErrorType_t validateToken(char* string){
    int length = strlen(string);
    char tokens[] = {'|', '<', '>'};
    int toklen = sizeof(tokens)/sizeof(char);
    // token at start of string
    for(int i = 0; i < length; i++){
        if(string[i] == ' '){
        }else if (string[i] == tokens[0]){
            printf("\033[0;31mCharacter: %c cannot appear as first token in a sequance\033[0m\n", tokens[0]);
            return Token_Err;
        }else{
            break;
        }
    }
    // token at end of string
    for(int i = length-1; i >= 0; i--){
        if(string[i] == ' '){
        }else{
            // go through token array to compare characters that need to be validated.
            for (int j = 0; j < toklen; j++){
                if (string[i] == tokens[j]){
                    if(tokens[j] == '>'){
                        printf("\033[0;31mCharacter: %c/%c%c cannot appear as last token in a sequance\033[0m\n",tokens[j],tokens[j] ,tokens[j]);
                    }else{
                        printf("\033[0;31mCharacter: %c cannot appear as last token in a sequance\033[0m\n", tokens[j]);
                    }
                    
                    return Token_Err;
                }
            }
            return Success;
        }
    }
    return Success;
}




ErrorType_t Decode_Redirect_out(char* string, PipeLine_t* pipeline){
    int length = strlen(string), i = 0, j = 0;

    char temp[length];
    // Skip all characters untill you reach a '>' character or the end of the array
    while(i<length){
        if(string[i] == '\\'){
            i +=2;
        }
        if(string[i] == '\"'){
            i++;
            while(i<length){
                if(string[i] == '\\'){
                    i +=2;
                }
                if(string[i] == '\"'){i++; break;}else{
                    i++;
                }
            }
        }if(strchr(">", string[i]) != NULL){
            string[i] = ' ';i++;
            // check whether the user specified >> or >
            if(strchr(">", string[i]) != NULL){
                pipeline->truncate = false;
                string[i] = ' ';i++;}else{pipeline->truncate = true;}
            break;
        }
        i++;
    }
    
    //printf("%d\n",i);
    // skip spaces until it reaches file names
    // example > hello.txt skips the space between the > and h.
    while(i<length){
        if(strchr(" ", string[i]) == NULL){break;}
        i++;
    }
    
    // Read File name
    while(i<length){
        if(string[i] == '\\'){
            string[i] = ' '; i++;
            // copy character into buffer
            temp[j] = string[i]; string[i] = ' '; i++; j++;
        }
        // name is in quotes
        if(string[i] == '\"'){
            i++;
            // copy character into buffer
            temp[j] = string[i]; string[i] = ' '; i++; j++;
            while(i<length){
                if(string[i] == '\\'){
                    string[i] = ' '; i++;
                    // copy character into buffer
                    temp[j] = string[i]; string[i] = ' '; i++; j++;
                }
                if(string[i] == '\"'){
                    i++; break;
                }else{
                    // copy character into buffer
                    temp[j] = string[i]; string[i] = ' '; i++; j++;
                }
            }
        }
        if(string[i] == ' '){
            break;
        }
        // copy character into buffer
        temp[j] = string[i]; string[i] = ' '; i++; j++;
    }
    
    // Save File Name if Found
    if(j>0){
        // allocate buffer into memory and save pointer to pipline->file_out at index
        temp[j]= '\0';
        int temp_size = sizeof(char)*(strlen(temp)+1);
        pipeline->file_out = (char*)malloc(temp_size);
        if(pipeline->file_out == NULL){return Malloc_Err;};
        strcpy(pipeline->file_out, temp);
    }else{
        pipeline->file_out = NULL;
    }
    return Success;
}





ErrorType_t Decode_Redirect_in(char* string, PipeLine_t* pipeline){
    int length = strlen(string), i = 0, j = 0;
    // Advance scanning
    bool found_quotation_mark = false;
    char temp[length];
    // Skip all characters untill you reach a '>' character or the end of the array
    while(i<length){
        if(string[i] == '\\'){
            i +=2;
        }
        if(string[i] == '\"'){
            i++;
            while(i<length){
                if(string[i] == '\\'){
                    i +=2;
                }
                if(string[i] == '\"'){i++; break;}else{
                    i++;
                }
            }
        }else if(strchr("<", string[i]) != NULL){
            string[i] = ' ';i++;
            break;
        }
        i++;
    }
    // skip spaces until it reaches file names
    // example < hello.txt skips the space between the < and h.
    while(i<length){
        if(strchr(" ", string[i]) == NULL){break;}
        i++;
    }

    // Read File name
    while(i<length){
        if(string[i] == '\\'){
            string[i] = ' '; i++;
            // copy character into buffer
            temp[j] = string[i]; string[i] = ' '; i++; j++;
        }
        // name is in quotes
        if(string[i] == '\"'){
            i++;
            // copy character into buffer
            temp[j] = string[i]; string[i] = ' '; i++; j++;
            while(i<length){
                if(string[i] == '\\'){
                    string[i] = ' '; i++;
                    // copy character into buffer
                    temp[j] = string[i]; string[i] = ' '; i++; j++;
                }
                if(string[i] == '\"'){
                    i++; break;
                }else{
                    // copy character into buffer
                    temp[j] = string[i]; string[i] = ' '; i++; j++;
                }
            }
        }
        if(string[i] == ' '){
            break;
        }
        // copy character into buffer
        temp[j] = string[i]; string[i] = ' '; i++; j++;
    }

    // Save File Name if Found
    if(j>0){
        // allocate buffer into memory and save pointer to pipline->file_in at index
        temp[j]= '\0';
        int temp_size = sizeof(char)*(strlen(temp)+1);
        pipeline->file_in = (char*)malloc(temp_size);
        if(pipeline->file_in == NULL){return Malloc_Err;};
        strcpy(pipeline->file_in, temp);
    }else{
        pipeline->file_in = NULL;
    }
    return Success;
}





ErrorType_t split_Program_into_Arguments(char* program, Arguments_t* argument_list){
    int length = strlen(program);
    argument_list->single_argument = NULL;
    argument_list->number_of_args = 0;
    Single_arg_t* next_t = argument_list->single_argument;
    char temp[length+1];
    int i = 0, j=0;
    while(i<length){
        //Skip spaces before reading an argument.
        while(i<length){
            if(program[i] == ' '){
                i++;
            }else{break;}
        }

        // READ ARGUEMENT
        while(i<length){
            // Check for back slash make sure it doesn't go out of bounds
            if(program[i] == '\\' && i <length-1){
                i++;
                // copy character into buffer
                temp[j] = program[i]; i++; j++;
            }
            // Arguement includes quotes
            if(program[i] == '\"'){
                
                i++;
                // copy character into buffer
                temp[j] = program[i]; i++; j++;
                while(i<length){
                    if(program[i] == '\\'){
                        i++;
                        // copy character into buffer
                        temp[j] = program[i]; i++; j++;
                    }
                    if(program[i] == '\"'){
                        i++; break;
                    }else{
                        // copy character into buffer
                        temp[j] = program[i]; i++; j++;
                    }
                }
            }
            if(program[i] == ' '){
                break;
            }
            // copy character into buffer
            temp[j] = program[i]; i++; j++;
        }
        
        // STORE ARGUEMENT
        if(j > 0){
            if(i == length-1 && program[i] != ' '){temp[j] = program[i];j++;}
            temp[j] = '\0';
            // First program in Queue
            if(argument_list->single_argument == NULL){
                next_t = argument_list->single_argument = (Single_arg_t*)malloc(sizeof(Single_arg_t));
            // Add to Queue
            }else{
                next_t->next_arg = (Single_arg_t*)malloc(sizeof(Single_arg_t));
                next_t = next_t->next_arg;
            }
            argument_list->number_of_args++;
            next_t->next_arg = NULL;
            next_t->s_arg = (char*)malloc(sizeof(char)*strlen(temp+1));
            strcpy(next_t->s_arg, temp);
            j=0;
        }else{i++;}
    }
}

char** Queue_to_String(Arguments_t* args){
    char** string = (char**)malloc(sizeof(char*)*args->number_of_args+1);
    Single_arg_t* temp = args->single_argument;
    int i=0;
    while(temp != NULL){
        string[i] = temp->s_arg;
        temp = temp->next_arg;
        i++;
    }
    string[args->number_of_args] = NULL;
    return string;
}

void freeArguments(Arguments_t* args){
    Single_arg_t* temp1 = args->single_argument;
    Single_arg_t* temp2 = temp1;
    for (size_t i = 0; i < args->number_of_args; i++){
        temp2 = temp1;
        if(i< args->number_of_args-1){
            temp1 = temp1->next_arg;
        }
        free(temp2->s_arg);
        free(temp2);
    }
}

void displayArgList(Arguments_t* a){
    Single_arg_t* temp = a->single_argument;
    int i = 1;
    while(temp != NULL){
        printf("\t\tArguement %d: %s\n",i , temp->s_arg);
        temp = temp->next_arg;
        i++;
    }
    printf("\t\tNumber of Args: %d\n", a->number_of_args);
}





ErrorType_t split_PipeLine_into_Programs(char* command, PipeLine_t* pipeline){
    int length = strlen(command);
    pipeline->args = NULL;
    pipeline->number_of_programs = 0;
    Arguments_t* next_a = pipeline->args;
    char temp[length+1];
    int i = 0, j=0;
    while(i<length){
        // If function finds a quotation mark it will automatically read all characters
        // until it finds the next quotation ignoring all metacharacters.
        if(command[i] == '\"'){
            temp[j] = command[i]; i++; j++;
            while(i<length){
                if(command[i] == '\\'){
                    temp[j] = command[i]; i++; j++;
                    temp[j] = command[i]; i++; j++;
                }else if(command[i] == '\"'){
                    temp[j] = command[i]; i++; j++;break;
                }else{
                    temp[j] = command[i]; i++; j++;
                }
            }
        }else if(command[i] == '\\' && i<length-1){
            temp[j] = command[i]; i++; j++;
            temp[j] = command[i]; i++; j++;
            
        }
        // Else, the program will copy characters untill it either reaches the end of the string or
        // the next piped program.
        // conditions to malloc : the loop meets a pipe symbol which is not followed by a string
        
        if((command[i] == '|') || i >= length-1){
            if(i == length-1){temp[j] = command[i];j++;} // read character at end of string before 
            i++; temp[j] = '\0';
            if(j == 0){
                return Token_Err;
            }else{
                // First program in Queue
                if(pipeline->args == NULL){
                    next_a = pipeline->args = (Arguments_t*)malloc(sizeof(Arguments_t));
                // Add to Queue
                }else{
                    next_a->next = (Arguments_t*)malloc(sizeof(Arguments_t));
                    next_a = next_a->next;
                }
                pipeline->number_of_programs ++;
                next_a->next = NULL;
                split_Program_into_Arguments(temp,next_a);
                if(next_a->number_of_args == 0){
                    return Token_Err;
                }
                j=0;
            }
        }else{
            temp[j] = command[i]; i++; j++;
        }
    }
    pipeline->number_of_pipes = pipeline->number_of_programs-1;
    return Success;
}
void freePipeline(PipeLine_t* pipeline){
    Arguments_t* temp1 = pipeline->args;
    Arguments_t* temp2;
    while(temp1 != NULL){
        temp2 = temp1;
        temp1 = temp1->next;
        freeArguments(temp2);
        free(temp2);
    }
    free(pipeline->command);
    free(pipeline->file_in);
    free(pipeline->file_out);
}
void displayPipeline(PipeLine_t* pipeline){
    Arguments_t* temp = pipeline->args;
    int i = 1;
    printf("Command: %s\n",pipeline->command);
    while(temp != NULL){
        printf("\tProgram %d: \n",i);
        displayArgList(temp);
        temp = temp->next;
        i++;
    }
    printf("\tNumber of programs: %d\n",pipeline->number_of_programs);
    printf("\tNumber of pipes: %d\n",pipeline->number_of_pipes);
    if(pipeline->truncate){printf("\tTruncate: True\n");}else{printf("\tTruncate: False\n");}
    printf("\tRedirect IN: %s\n",pipeline->file_in);
    printf("\tRedirect OUT: %s\n",pipeline->file_out);
}