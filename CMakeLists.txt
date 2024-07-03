cmake_minimum_required(VERSION 3.22)
project(practicle)

        
add_compile_options(-Wno-unused)

add_executable(Shell Shell.c linenoise.c Builtin.c 
               ErrorHandler.c ExecuteFunctions.c DecodePipesAndArgs.c
               EnumsAndUtility.c InputOutputRedirect.c AdvancedDecode.c)