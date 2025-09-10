vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NewYaroslav/time-shield-cpp
    REF v1.0.2
    SHA512 94e226fbef842599fdb2c88e0a3c38baddf8f0c02606e5533b2f7e6cb518c3592c66ef29088d34b2444712276af13f0492c326c551e5c0712b79a2a86cddbca8
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DTIME_SHIELD_CPP_BUILD_EXAMPLES=OFF
        -DTIME_SHIELD_CPP_BUILD_TESTS=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME TimeShield
    CONFIG_PATH lib/cmake/TimeShield
)

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME LICENSE)
file(INSTALL ${CMAKE_CURRENT_LIST_DIR}/vcpkg-cmake-wrapper.cmake DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT})

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug)
