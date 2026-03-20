set(ODR_SOURCES
    a.cpp
    b.cpp
)

add_executable(odr_cxx11 ${ODR_SOURCES})
target_link_libraries(odr_cxx11 PRIVATE time_shield::time_shield)
set_target_properties(odr_cxx11 PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

add_test(NAME odr_cxx11 COMMAND odr_cxx11)

add_executable(odr_cxx14 ${ODR_SOURCES})
target_link_libraries(odr_cxx14 PRIVATE time_shield::time_shield)
set_target_properties(odr_cxx14 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)

add_test(NAME odr_cxx14 COMMAND odr_cxx14)

add_executable(odr_cxx17 ${ODR_SOURCES})
target_link_libraries(odr_cxx17 PRIVATE time_shield::time_shield)
set_target_properties(odr_cxx17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

add_test(NAME odr_cxx17 COMMAND odr_cxx17)

set(NTP_TIME_SERVICE_ODR_SOURCES
    ntp_time_service_a.cpp
    ntp_time_service_b.cpp
)

add_executable(ntp_time_service_odr_cxx11 ${NTP_TIME_SERVICE_ODR_SOURCES})
target_link_libraries(ntp_time_service_odr_cxx11 PRIVATE time_shield::time_shield)
set_target_properties(ntp_time_service_odr_cxx11 PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

add_test(NAME ntp_time_service_odr_cxx11 COMMAND ntp_time_service_odr_cxx11)

add_executable(ntp_time_service_odr_cxx14 ${NTP_TIME_SERVICE_ODR_SOURCES})
target_link_libraries(ntp_time_service_odr_cxx14 PRIVATE time_shield::time_shield)
set_target_properties(ntp_time_service_odr_cxx14 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)

add_test(NAME ntp_time_service_odr_cxx14 COMMAND ntp_time_service_odr_cxx14)

add_executable(ntp_time_service_odr_cxx17 ${NTP_TIME_SERVICE_ODR_SOURCES})
target_link_libraries(ntp_time_service_odr_cxx17 PRIVATE time_shield::time_shield)
set_target_properties(ntp_time_service_odr_cxx17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

add_test(NAME ntp_time_service_odr_cxx17 COMMAND ntp_time_service_odr_cxx17)
