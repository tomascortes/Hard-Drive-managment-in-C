cmake_minimum_required(VERSION 3.22)
project(P1_SSO C)

set(CMAKE_C_STANDARD 99)

add_executable(P1_SSO src/ssdfs/main.c src/ssdfs/os_API.c src/ssdfs/os_API.h src/ssdfs/aux/os_file.c src/ssdfs/aux/os_file.h src/ssdfs/debug/debug.h src/ssdfs/debug/debug.c src/ssdfs/aux/auxiliary_fx.c src/ssdfs/aux/auxiliary_fx.h src/ssdfs/aux/directree.c src/ssdfs/aux/directree.h)