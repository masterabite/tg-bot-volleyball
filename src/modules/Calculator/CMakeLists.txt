cmake_minimum_required(VERSION 3.11.1)
project(Calculator)

add_library(Calculator SHARED 
    Calculator.cpp
)


set(INSTALL_PATH usr/lib/)

install(TARGETS Calculator LIBRARY DESTINATION ${INSTALL_PATH})