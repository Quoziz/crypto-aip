#
# Project settings
#
SET(${PROJECT_NAME}_BUILD_EXECUTABLE ON)

#
# Static analyzers
#
SET(${PROJECT_NAME}_ENABLE_CLANG_TIDY OFF)

#
# Code coverage
#
set(${PROJECT_NAME}_ENABLE_CODE_COVERAGE ON)

#
# Doxygen
#
SET(${PROJECT_NAME}_ENABLE_DOXYGEN ON)

#
# Miscelanious options
#

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(${PROJECT_NAME}_VERBOSE_OUTPUT "Enable verbose output, allowing for a better understanding of each step taken." ON)