# Tests use GoogleTest (C++)
enable_language(CXX)

option(FORCE_FETCH_DEPS "Force downloading dependencies even if found on system" OFF)

if(NOT FORCE_FETCH_DEPS)
    find_package(GTest QUIET)
endif()

if(NOT GTest_FOUND)
    message(STATUS "GTest not found locally, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
      DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Alias GoogleTest targets with project prefix for better identification
add_library(${PROJECT_NAME}-gmock_main INTERFACE)
if(GTest_FOUND)
    target_link_libraries(${PROJECT_NAME}-gmock_main INTERFACE GTest::gmock_main)
else()
    target_link_libraries(${PROJECT_NAME}-gmock_main INTERFACE gmock_main)
endif()

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

if(TEST_SOURCES)
    add_executable(${PROJECT_NAME}-test ${TEST_SOURCES})

    target_compile_options(${PROJECT_NAME}-test PRIVATE --coverage -O0 -g)
    target_link_options(${PROJECT_NAME}-test PRIVATE --coverage)

    # Link against GTest and the app's dependencies
    if(TARGET caffeine::hal-mock)
        set(HAL_DEP caffeine::hal caffeine::hal-mock)
    else()
        set(HAL_DEP caffeine::hal-ports)
    endif()

    target_link_libraries(${PROJECT_NAME}-test
        PRIVATE
        ${PROJECT_NAME}-gmock_main
        ${HAL_DEP}
    )

    # Include app and bsp headers
    target_include_directories(${PROJECT_NAME}-test PRIVATE
        ${CMAKE_SOURCE_DIR}/src/app
        ${CMAKE_SOURCE_DIR}/src/bsp
    )

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}-test)

    find_program(GCOVR_PATH gcovr)
    if(GCOVR_PATH)
        add_custom_target(${PROJECT_NAME}-coverage
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${GCOVR_PATH}
                --root ${CMAKE_SOURCE_DIR}
                --filter "${CMAKE_SOURCE_DIR}/src/"
                --exclude-unreachable-branches
                --gcov-ignore-errors=no_working_dir_found
                --print-summary
                --html-details coverage.html
                --txt coverage.txt
                ${CMAKE_BINARY_DIR}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${PROJECT_NAME}-test
            COMMENT "Running tests and generating coverage report (HTML and TXT)"
        )
    endif()
endif()
