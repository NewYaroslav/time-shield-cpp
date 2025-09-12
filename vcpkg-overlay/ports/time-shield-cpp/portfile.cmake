vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NewYaroslav/time-shield-cpp
    REF ff3852144b64d9aea9ba3c4b0116016940af1642
    SHA512 a2afdeddaf688214b092d508d3965859b6f44534aec35058089c0f89fb03ddb3d09ba11903af078e976a408942ce67cdc80084c99d5f10f858b2247826570cb7
    HEAD_REF stable
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DTIME_SHIELD_CPP_BUILD_EXAMPLES=OFF
        -DTIME_SHIELD_CPP_BUILD_TESTS=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME TimeShield
    CONFIG_PATH lib/cmake/TimeShield
)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
    "${CURRENT_PACKAGES_DIR}/debug/lib"
    "${CURRENT_PACKAGES_DIR}/debug/share/${PORT}"
)
