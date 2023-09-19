cmake_minimum_required(VERSION 3.15)

#
# Project details
#

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

enable_testing()

include(../cmake/CodeCoverage.cmake)

message(STATUS "Fetching googletest")
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)


SET(BUILD_GMOCK ON CACHE BOOL "" FORCE)
SET(BUILD_GTEST ON CACHE BOOL "" FORCE)
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

verbose_message("Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

set(CMAKE_CXX_STANDARD 17)

add_executable(${CMAKE_PROJECT_NAME}Tests ${test_sources})

include(GoogleTest)

set(GOOGLE_MOCK_LIBRARIES GTest::gmock GTest::gmock_main)

target_link_libraries(
  ${CMAKE_PROJECT_NAME}Tests
  PRIVATE
  ${CMAKE_PROJECT_NAME}_LIB
)

target_link_libraries(
  ${CMAKE_PROJECT_NAME}Tests
  PUBLIC
    gtest
    gtest_main
    ${GOOGLE_MOCK_LIBRARIES}
)

target_compile_options(${CMAKE_PROJECT_NAME}Tests PRIVATE)

# include_directories(${CMAKE_PROJECT_NAME}/include)
#
# Add the unit tests
#

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(COVERAGE_EXCLUDES ".*gtest.*" ".*googletest.*" ".*test.*")
else()
  set(COVERAGE_EXCLUDES "${CMAKE_SOURCE_DIR}/build/*" "${CMAKE_SOURCE_DIR}/test/*" "include/gtest/*")
endif()

gtest_discover_tests(${CMAKE_PROJECT_NAME}Tests)

target_code_coverage(
  ${CMAKE_PROJECT_NAME}Tests
  EXCLUDE ${COVERAGE_EXCLUDES} # Files to EXCLUDE.
  AUTO
)

verbose_message("Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")