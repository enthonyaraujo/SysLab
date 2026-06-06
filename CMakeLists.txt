cmake_minimum_required(VERSION 3.16)

project(SysLab VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(SysLab
    main.cpp

    ui/mainwindow.cpp
    ui/mainwindow.hpp

    src/processos.cpp
    include/processos.hpp

    src/escalonador.cpp
    include/escalonador.hpp

    src/memoria.cpp
    include/memoria.hpp

    src/simulador.cpp
    include/simulador.hpp
)

target_include_directories(SysLab PRIVATE
    ui
    include
)

target_link_libraries(SysLab PRIVATE Qt6::Widgets)