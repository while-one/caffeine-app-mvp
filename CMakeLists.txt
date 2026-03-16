cmake_minimum_required(VERSION 3.23)

# 1. Load Caffeine Build Macros (before project() so toolchains are available if needed)
include(caffeine-build/cmake/CaffeineMacros.cmake)

project(caffeine-app-mvp
        VERSION 0.1.0
        LANGUAGES C ASM
        DESCRIPTION "Minimum Viable Product Blinky Application"
        HOMEPAGE_URL "https://github.com/while-one/caffeine-app-mvp"
)

# Global standards for the Caffeine Framework
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# User-configurable flags
option(CAFFEINE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
set(CAFFEINE_OPTIMIZATION_LEVEL "-Os" CACHE STRING "Compiler optimization level")

set(CAFFEINE_COMPILE_OPTIONS
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
    GIT_TAG        main # Or a specific tag/branch like refactor-build
)
FetchContent_MakeAvailable(caffeine-hal-ports)

# --- 3. Source Collection ---
set(APP_SOURCES src/main.c)

# --- 4. Application Executable ---
add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CAFFEINE_COMPILE_OPTIONS})
target_link_libraries(${PROJECT_NAME} PRIVATE caffeine::hal-ports)

# 5. Generate hex/bin and print size
cfn_add_firmware(${PROJECT_NAME})

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
# This will be refined once the full source tree is created in Phase 2/3
add_custom_target(${PROJECT_NAME-analyze} COMMENT "Running full static analysis suite...")
