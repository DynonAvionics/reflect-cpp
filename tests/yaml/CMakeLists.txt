project(reflect-cpp-yaml-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-yaml-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-yaml-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-yaml-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-yaml-tests 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-yaml-tests)

