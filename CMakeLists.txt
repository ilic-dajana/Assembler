cmake_minimum_required(VERSION 3.12)
project(SS_project C)

set(CMAKE_C_STANDARD 11)

include_directories(h)

add_executable(SS_project
    h/data_types.h
    h/error.h
    h/firstpass.h
    h/helper.h
    h/lexer.h
    h/parser.h
    h/secondpass.h
    h/tables.h
    src/error.c
    src/firstpass.c
    src/helper.c
    src/lexer.c
    src/lexertest.c
    src/main.c
    src/parser.c
    src/secondpass.c
    src/tables.c)
