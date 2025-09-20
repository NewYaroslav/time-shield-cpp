set(ODR_MWE_SOURCES
    a.cpp
    b.cpp
)

add_executable(odr_mwe_cxx11 ${ODR_MWE_SOURCES})
target_link_libraries(odr_mwe_cxx11 PRIVATE time_shield::time_shield)
set_target_properties(odr_mwe_cxx11 PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

add_executable(odr_mwe_cxx17 ${ODR_MWE_SOURCES})
target_link_libraries(odr_mwe_cxx17 PRIVATE time_shield::time_shield)
set_target_properties(odr_mwe_cxx17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
