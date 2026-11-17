cmake_minimum_required(VERSION 4.1)
project(Group_4_DAA C)

set(CMAKE_C_STANDARD 11)

add_executable(Group_4_DAA
        end_of_sem_algo_explorer/gtk_gui.c
        end_of_sem_algo_explorer/main.c
        end_of_sem_algo_explorer/algorithms.c
        end_of_sem_algo_explorer/datastructures.c
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
target_link_libraries(Group_4_DAA PRIVATE PkgConfig::GTK4)

include(CTest)
if(BUILD_TESTING)
    add_executable(stepped_algorithms_test
            end_of_sem_algo_explorer/algorithms.c
            tests/stepped_algorithms_test.c
    )
    target_include_directories(stepped_algorithms_test PRIVATE
            end_of_sem_algo_explorer
    )
    target_link_libraries(stepped_algorithms_test PRIVATE PkgConfig::GTK4)
    add_test(NAME stepped_algorithms_test COMMAND stepped_algorithms_test)
endif()
