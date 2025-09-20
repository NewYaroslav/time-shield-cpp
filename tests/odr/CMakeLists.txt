set(ODR_SOURCES
    a.cpp
    b.cpp
)

add_executable(odr_cxx11 ${ODR_SOURCES})
target_link_libraries(odr_cxx11 PRIVATE time_shield::time_shield)
set_target_properties(odr_cxx11 PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

add_test(NAME odr_cxx11 COMMAND odr_cxx11)

add_executable(odr_cxx17 ${ODR_SOURCES})
target_link_libraries(odr_cxx17 PRIVATE time_shield::time_shield)
set_target_properties(odr_cxx17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

add_test(NAME odr_cxx17 COMMAND odr_cxx17)
