#include "EnumsAndUtility.h"



int count_Sub_Strings(char* string, int total_length, char* split_by){
    int number_of_subStrings = 0, i = 0, prev_i;
    while(i<total_length){
        // skip any [split_by] characters at the start of a string
        while(i<total_length){
            if(strchr(split_by, string[i]) == NULL){break;}
            i++;
        }
        prev_i = i;
        // read an characters until reach the next [split_by] character
        while(i< total_length){
            if(strchr(split_by, string[i]) != NULL){break;}
            i++;
        }
        if(i>prev_i){number_of_subStrings++;}
    }
    return number_of_subStrings;
}
