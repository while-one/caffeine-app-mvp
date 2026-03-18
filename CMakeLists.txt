cmake_minimum_required(VERSION 3.23)

# Placeholder project name and version
project(caffeine-app-mvp
        VERSION 0.0.1
        LANGUAGES C ASM
        DESCRIPTION "Minimum Viable Product Blinky Application"
        HOMEPAGE_URL "https://github.com/while-one/caffeine-app-mvp"
)

# Global standards for the Caffeine Framework
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(${PROJECT_SOURCE_DIR}/caffeine-build/cmake/CaffeineMacros.cmake)

# User-configurable flags
option(CAFFEINE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
set(CAFFEINE_OPTIMIZATION_LEVEL "-Os" CACHE STRING "Compiler optimization level")

set(CAFFEINE_COMPILE_OPTIONS
    -pedantic-errors
    -Wall -Wextra -Wpedantic -Wshadow
    -ffunction-sections -fdata-sections
    ${CAFFEINE_OPTIMIZATION_LEVEL}
)

if(CAFFEINE_WARNINGS_AS_ERRORS)
    list(APPEND CAFFEINE_COMPILE_OPTIONS -Werror)
endif()

# Useful for IDEs, clang-tidy, and clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- 2. Dependency: Caffeine-HAL Ports ---
include(FetchContent)
FetchContent_Declare(
    caffeine-hal-ports
    GIT_REPOSITORY https://github.com/while-one/caffeine-hal-ports.git
    GIT_TAG        v0.2.4
    GIT_SUBMODULES
    "caffeine-build"
)
FetchContent_MakeAvailable(caffeine-hal-ports)

# --- 3. Source Collection ---
set(APP_SOURCES 
    src/main.c
    src/app/app_main.c
    src/bsp/bsp_led.c
)

# --- 4. Application Executable ---
add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE ${CAFFEINE_COMPILE_OPTIONS})
target_include_directories(${PROJECT_NAME} PRIVATE 
    src/app
    src/bsp
)

# Inherit HAL and Port-specific flags (CPU, FPU, includes, linker scripts)
target_link_libraries(${PROJECT_NAME} PRIVATE caffeine::hal-ports)

# Inject Board ID as a preprocessor definition if set
if(CFN_APP_BOARD_ID)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "${CFN_APP_BOARD_ID}")
endif()

# 5. Generate hex/bin and print size
cfn_add_firmware(${PROJECT_NAME})

# --- 6. Unit Testing ---
if(PROJECT_IS_TOP_LEVEL)
    set(DEFAULT_BUILD_TESTS ON)
else()
    set(DEFAULT_BUILD_TESTS OFF)
endif()
option(CFN_APP_MVP_BUILD_TESTS "Build the unit tests for MVP app" ${DEFAULT_BUILD_TESTS})

if(CFN_APP_MVP_BUILD_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

# ==============================================================================
# Tooling: Code Formatting & Static Analysis
# ==============================================================================

# 1. Code Formatting (clang-format)
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_FORMAT_SOURCES "src/**/*.c" "src/**/*.h")
    if(ALL_FORMAT_SOURCES)
        add_custom_target(
            ${PROJECT_NAME}-format
            COMMAND ${CLANG_FORMAT} -i
            -style=file:${PROJECT_SOURCE_DIR}/caffeine-build/config/coding/.clang-format
            ${ALL_FORMAT_SOURCES}
            COMMENT "Formatting application files..."
        )
    endif()
endif()

# 2. Static Analysis (analyze)
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(
        ${PROJECT_NAME}-cppcheck
        COMMAND ${CPPCHECK}
        --project=${CMAKE_BINARY_DIR}/compile_commands.json
        --enable=all
        --suppress=missingInclude
        --suppress=missingIncludeSystem
        --suppress=unusedFunction
        --suppress=*:*/_deps/*
        --inline-suppr
        --std=c11
        --file-filter=${PROJECT_SOURCE_DIR}/src/*
        COMMENT "Running cppcheck static analysis using compile_commands.json..."
    )
endif()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    add_custom_target(
        ${PROJECT_NAME}-tidy
        COMMAND ${CLANG_TIDY}
        -p=${CMAKE_BINARY_DIR}
        --config-file=${PROJECT_SOURCE_DIR}/caffeine-build/config/coding/.clang-tidy
        --header-filter="${PROJECT_SOURCE_DIR}/src/.*"
        ${ALL_FORMAT_SOURCES}
        COMMENT "Running clang-tidy static analysis using compile_commands.json..."
    )
endif()

add_custom_target(${PROJECT_NAME}-analyze COMMENT "Running full static analysis suite...")
if(TARGET ${PROJECT_NAME}-cppcheck)
    add_dependencies(${PROJECT_NAME}-analyze ${PROJECT_NAME}-cppcheck)
endif()
if(TARGET ${PROJECT_NAME}-tidy)
    add_dependencies(${PROJECT_NAME}-analyze ${PROJECT_NAME}-tidy)
endif()


