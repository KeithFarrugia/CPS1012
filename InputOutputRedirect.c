#include "ExecuteFunctions.h"

bool file_exists(const char *f){
    bool e = true;
    if(f != NULL){
        FILE *fp = fopen(f, "r");
        // Check if file exists
        if (fp == NULL){
            e = false;
        // Close the file
        }else{fclose(fp);}
    }
    return e;
}



ErrorType_t reopen(int fd, char *path, int flags, mode_t mode){
    int open_fd = open(path, flags, mode); // returns file descriptor
    // if returned file descriptor matches the file descripter passed as arguement or error then function ends
    if(open_fd == fd || open_fd < 0){ 
        return Redirect_Err;
    }
    int dub_fd = dup2(open_fd, fd);
    close(open_fd);
    return dub_fd == -1 ? Redirect_Err : Success;
}



ErrorType_t redirect_input(char *input){
    return(reopen(STDIN_FILENO, input, O_RDONLY, S_IRUSR));
}


ErrorType_t redirect_output(char *output, bool trunc){
    
    // Append or truncate depending on [trunc] 
    if(trunc == true){
        return(reopen(STDOUT_FILENO, output, O_RDWR | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP));
    }else{
        return(reopen(STDOUT_FILENO, output, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP));
    }
}