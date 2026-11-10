cmake_minimum_required(VERSION 4.1)
project(Group_4_DAA C)

set(CMAKE_C_STANDARD 11)

add_executable(Group_4_DAA
        end_of_sem_algo_explorer/main.c
        end_of_sem_algo_explorer/algorithms.c)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
target_link_libraries(Group_4_DAA PRIVATE PkgConfig::GTK4)
