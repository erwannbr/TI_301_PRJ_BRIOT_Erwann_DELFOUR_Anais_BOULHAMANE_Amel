cmake_minimum_required(VERSION 3.27)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PJT
        main.c utils/utils.c
        cell/cell.c list/list.c
        adjacency_list/adjacency_list.c
        tarjan/tarjan.c)
