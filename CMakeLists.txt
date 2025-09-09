cmake_minimum_required(VERSION 3.18)
project(time_shield_cpp VERSION 0.1.0 LANGUAGES CXX)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(time_shield_cpp INTERFACE)
add_library(time_shield_cpp::time_shield_cpp ALIAS time_shield_cpp)

target_compile_features(time_shield_cpp INTERFACE cxx_std_11)

target_include_directories(
    time_shield_cpp
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/time_shield_cpp>
        $<INSTALL_INTERFACE:include/time_shield_cpp>
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS time_shield_cpp EXPORT time_shield_cppTargets)
install(DIRECTORY include/time_shield_cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT time_shield_cppTargets
    FILE time_shield_cppTargets.cmake
    NAMESPACE time_shield_cpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/time_shield_cpp
)

configure_package_config_file(
    cmake/time_shield_cppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/time_shield_cppConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/time_shield_cpp
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/time_shield_cppConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/time_shield_cppConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/time_shield_cppConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/time_shield_cpp
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(is_top_level ON)
else()
    set(is_top_level OFF)
endif()

option(TIME_SHIELD_CPP_BUILD_EXAMPLES "Build examples" ${is_top_level})
option(TIME_SHIELD_CPP_BUILD_TESTS "Build tests" ${is_top_level})

if(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMMON_WARN_FLAGS -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow)
endif()

if(MSVC)
    set(COMMON_WARN_FLAGS /W4 /wd4996)
endif()

file(GLOB_RECURSE PROJECT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.hpp)
add_custom_target(project_headers SOURCES ${PROJECT_HEADERS})

if(TIME_SHIELD_CPP_BUILD_EXAMPLES)
    file(GLOB EXAMPLES_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/*.cpp)
    foreach(example_src ${EXAMPLES_SOURCES})
        get_filename_component(example_name ${example_src} NAME_WE)
        add_executable(${example_name} ${example_src})
        target_link_libraries(${example_name} PRIVATE time_shield_cpp::time_shield_cpp)
        if(COMMON_WARN_FLAGS)
            target_compile_options(${example_name} PRIVATE ${COMMON_WARN_FLAGS})
        endif()
        if(WIN32 AND example_name STREQUAL "ntp_client_example")
            target_link_libraries(${example_name} PRIVATE ws2_32)
        endif()
        set_source_files_properties(${PROJECT_HEADERS} PROPERTIES HEADER_FILE_ONLY ON)
        target_sources(${example_name} PRIVATE ${PROJECT_HEADERS})
    endforeach()
endif()

if(TIME_SHIELD_CPP_BUILD_TESTS)
    enable_testing()
    file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)
    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        add_executable(${test_name} ${test_src})
        target_link_libraries(${test_name} PRIVATE time_shield_cpp::time_shield_cpp)
        if(COMMON_WARN_FLAGS)
            target_compile_options(${test_name} PRIVATE ${COMMON_WARN_FLAGS})
        endif()
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
