cmake_minimum_required(VERSION 3.1.0)
project(SherkSystem C)

set(CMAKE_C_STANDARD 99)


link_libraries(libmsgpackc.a)

set(INCLUDE_DIR ./include)
set(MODULE_DIR ./module)
add_executable(sherkSystem system.c

                ${INCLUDE_DIR}/define/const.h
        #        ${INCLUDE_DIR}/define/exitcode.h
                ${INCLUDE_DIR}/define/message.h
                ${INCLUDE_DIR}/define/rescode.h
        )
