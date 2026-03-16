# Tests use GoogleTest (C++)
enable_language(CXX)

find_package(GTest QUIET)

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

file(GLOB_RECURSE TEST_SOURCES "app/**/*.cpp")

if(TEST_SOURCES)
    add_executable(${PROJECT_NAME}-test ${TEST_SOURCES})

    target_compile_options(${PROJECT_NAME}-test PRIVATE -O0 -g)

    # Link against GTest and the app's dependencies
    # For MVP we'll link against the same libs as the app
    target_link_libraries(${PROJECT_NAME}-test
        PRIVATE
        GTest::gtest_main
        caffeine::hal-ports
    )

    # Include app and bsp headers
    target_include_directories(${PROJECT_NAME}-test PRIVATE
        ${CMAKE_SOURCE_DIR}/src/app
        ${CMAKE_SOURCE_DIR}/src/bsp
    )

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}-test)
endif()
