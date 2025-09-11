cmake_minimum_required(VERSION 3.15)
project(InstallConsumer LANGUAGES CXX)

find_package(TimeShield CONFIG REQUIRED)

add_executable(install_consumer consumer.cpp)
target_link_libraries(install_consumer PRIVATE time_shield::time_shield)

if(MSVC)
    target_compile_options(install_consumer PRIVATE /W4)
else()
    target_compile_options(install_consumer PRIVATE -Wall -Wextra -Wpedantic)
endif()
