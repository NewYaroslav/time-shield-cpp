vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NewYaroslav/time-shield-cpp
    REF v1.0.4
    SHA512 b52c0cedf50b2afe5bc97ad5f266a474769b97d6ee2c205501644d8a58e107bd69efdf66bf6bb01466d3e670174ee08f1a85a987b92ca9b110af6ccd1f4dcfbd
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
