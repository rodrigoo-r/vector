cmake_minimum_required(VERSION 3.12)
project(vector C)

set(CMAKE_C_STANDARD 11)

add_library(vector STATIC vector.c)
