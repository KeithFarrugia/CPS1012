#include "ExecuteFunctions.h"



ErrorType_t wait_for_child(pid_t pid){
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        // Wait Faild
        return Wait_Err;
    }
    // Get exit status of child
    if ( WIFEXITED(status)) {        
        int exit_stat = WEXITSTATUS(status);
        if(exit_stat == 0){
            return Success;
        }else{
            return Exec_Err;
        }
    }
}



ErrorType_t fork_exec(char *const *args, pid_t* pid_child) {
    // Basic error checking
    if (args == NULL || *args == NULL) {return EmptyArgs_Err;}
    //===================Fork=============================
    // create fork and check if the creation works
    pid_t pid = fork();
    if (pid == -1) {return Fork_Err;}
    //===================Execute=============================
    if (pid == 0) {
        //Child
        if(execvp(args[0], args) == -1){
            exit(Exec_Err);
        }
        exit(Success);
    }else{*pid_child = pid;}
}




ErrorType_t fork_exec_pipe(char ** program, pid_t* pid_child, 
                           int pipe_fd_in[2], int pipe_fd_out[2],
                           char* file_in, char* file_out, bool truncate) {
    //Check arguments
    if (program == NULL || *program == NULL) {return EmptyArgs_Err;}

    //===================Fork=============================
    // create fork and check if the creation works
    pid_t pid = fork();
    if (pid == -1) {
        return Fork_Err;
    }else if (pid == 0){ // CHILD
        // If the input pipe is NULL then it is assumed 
        // that program is the first in a chain of pipes
        if(pipe_fd_in != NULL) {
            close(pipe_fd_in[WRITE]); // close the WRITE entry of the input pipe (set to READ)
            dup2(pipe_fd_in[READ], STDIN_FILENO);
            close(pipe_fd_in[READ]);
        }else if(file_in != NULL){
            redirect_input(file_in);
        }
        // if the output pipe is NULL then it is assumed
        // that the program is the end of a chain of pipes
        if(pipe_fd_out != NULL){
            close(pipe_fd_out[READ]); // close the READ entry of the input pipe (set to WRITE)
            dup2(pipe_fd_out[WRITE], STDOUT_FILENO);
            close(pipe_fd_out[WRITE]);
        }else if(file_out != NULL){
            redirect_output(file_out, truncate);
        }
        //Execute program Arguments
        if(execvp(program[0], program) == -1){exit(Exec_Err);}
    }else{
        // disconnect parent from pipe
        if(pipe_fd_in != NULL){
            close(pipe_fd_in[READ]);
            close(pipe_fd_in[WRITE]);
        }
        *pid_child = pid; 
    }
    return Success;
}






ErrorType_t execute_pipeline(PipeLine_t* pipeline, bool async){
    ErrorType_t f, w;
    pid_t child_pid;
    
    char** string_of_args;
    Arguments_t* temp_arg;
    temp_arg = pipeline->args;
    
    // Check if pipeline is empty
    if(pipeline->number_of_pipes != 0){
        // Array of pipes
        int pipe_fd_array[pipeline->number_of_pipes][2];
        int *left_pipe = NULL, *right_pipe = NULL;

        //Loop through queue running each program piping each one into the next
        for (int i = 0; i < pipeline->number_of_programs; i++){

            string_of_args = Queue_to_String(temp_arg);
            left_pipe = right_pipe;
            // First Progam in pipeline
            if(i == 0){
                right_pipe = pipe_fd_array[i];
                if(pipe(right_pipe) == -1){freePipeline(pipeline);return Pipe_Err;}; 
                f = fork_exec_pipe(string_of_args, &child_pid, left_pipe, right_pipe, pipeline->file_in, NULL, false);
            // Last Program in pipeline
            }else if(i == pipeline->number_of_programs-1){
                right_pipe = NULL;
                f = fork_exec_pipe(string_of_args, &child_pid, left_pipe, right_pipe, NULL, pipeline->file_out, pipeline->truncate);
                //If async is false and its the last program in the pipeline the parent waits for the program
                if(async == false){
                    w = wait_for_child(child_pid);
                    if(w != Success){return w;}
                }
            // Any programs in the middle
            }else{
                right_pipe = pipe_fd_array[i];
                if(pipe(right_pipe) == -1){freePipeline(pipeline);return Pipe_Err;}; 
                f = fork_exec_pipe(string_of_args, &child_pid, left_pipe, right_pipe, NULL, NULL, false);
            }
            // Error Checking
            if(f != Success){freePipeline(pipeline);return f;}
            free(string_of_args);
            temp_arg = temp_arg->next;
        }
    }else{
        string_of_args = Queue_to_String(temp_arg);
        f = fork_exec_pipe(string_of_args, &child_pid, NULL, NULL, pipeline->file_in, pipeline->file_out, pipeline->truncate);
        if(f != Success){freePipeline(pipeline);return f;};
        if(async == false){
            w = wait_for_child(child_pid);
            if(w != Success){return w;}
        }
        
        temp_arg = temp_arg->next;
        free(string_of_args);
    }
    freePipeline(pipeline);
    return Success;
}
