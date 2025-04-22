# При генерации Code::Blocks проекта с MinGW можно использовать так:
# cmake -G "CodeBlocks - MinGW Makefiles" -S . -B build-cb
cmake_minimum_required(VERSION 3.18)
project(time_shield LANGUAGES CXX)

# Пути к инклудам и библиотекам
set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/time_shield_cpp
)

# Заголовочные файлы из include/
file(GLOB_RECURSE PROJECT_HEADERS
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    include/*.hpp
)

# Создаём виртуальную цель для заголовков, чтобы отображались в IDE
add_custom_target(project_headers SOURCES ${PROJECT_HEADERS})

# Находим все .cpp файлы в examples/
file(GLOB EXAMPLES_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/*.cpp)

# Для каждого создаём отдельную цель
foreach(example_src ${EXAMPLES_SOURCES})
    get_filename_component(example_name ${example_src} NAME_WE)

    add_executable(${example_name} ${example_src})

    target_include_directories(${example_name} PRIVATE ${PROJECT_INCLUDE_DIRS})
    target_link_directories(${example_name} PRIVATE ${PROJECT_LIBRARY_DIRS})
    target_compile_definitions(${example_name} PRIVATE ${PROJECT_DEFINES})
    target_link_libraries(${example_name} PRIVATE ${PROJECT_LIBS})
	
	# Добавляем заголовочные файлы в IDE-представление цели
	set_source_files_properties(${PROJECT_HEADERS} PROPERTIES HEADER_FILE_ONLY ON)
    target_sources(${example_name} PRIVATE ${PROJECT_HEADERS})
	
	foreach(dll ${DLL_FILES})
        add_custom_command(TARGET ${example_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dll}"
            "$<TARGET_FILE_DIR:${example_name}>"
        )
    endforeach()
endforeach()
