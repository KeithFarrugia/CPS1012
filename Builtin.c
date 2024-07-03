#include "Builtin.h"

/* =================================================================================
 * Custom Bultin Functions
*/
ErrorType_t buitin_exit ( char ** args ) {
    exit(EXIT_SUCCESS);
    return Success;
}
ErrorType_t buitin_cd ( char ** args ) {
    int x = chdir(args[1]);
    if(x == -1){
       return Set_Directory_Err;
    }
    return Success;
}
ErrorType_t buitin_cwd ( char ** args ) {
    char s[PATH_MAX];
    getcwd(s,PATH_MAX);
    printf("CURRENT DIRECTORY : %s\n",s);
    //getenv(CWD) returns a null and CWD was not listed in the env variables so this function is used instead
    return Success;
}
ErrorType_t buitin_ver  ( char ** args ) {
    printf("\nAuthor: Keith Farrugia\n");
    printf("Shell Version: 1.0.0\n");
    printf("Remade around twice about 1000 lines of code (nothing to write home about).\n");
    printf("Named after arguably the best starter in gen 1 (no its not Charmander).\n");
    printf("Is it perfect? Definitly not.\n");
    printf("Does it work? Maybe not entirely sure (wouldn't bet any money on it.)\n");
    printf("Should I rebuild it now that I have a better idea what im doing? Perhaps but time isn't on my side.\n");
    printf("Its a patchwork of a Shell and you can't be sane if you actually try to use it.\n");
    printf("But I had fun making it and I'm pleased with it for now :)\n\n");
    return Success;
}

/* =================================================================================
 * Custom Bultin Command array
*/
builtin_command_t builtin_list [] = {
    { "exit" , &buitin_exit},
    { "cd"   , &buitin_cd},
    { "cwd"  , &buitin_cwd},
    { "ver"  , &buitin_ver}
};


/* =================================================================================
 * Decode a given function to check whether it contains a custom command
 * returns true if a built in command matching that of the input string is found
 */
bool compare_builtin(char** string){
    if (string != NULL){
        int num_of_cmds = sizeof(builtin_list)/sizeof(builtin_command_t);
        for(int i = 0; i < num_of_cmds; i++){
            if(strncmp(builtin_list[i].name, string[0], 10) == 0){
                handleErr(builtin_list[i].method(string));
                return true;
            };
        }
    }
    return false;
};