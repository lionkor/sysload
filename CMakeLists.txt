cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(sysload
    VERSION 1.0.0
    LANGUAGES CXX)

# ------------------------ APPLE ---------------------------------
if(APPLE)
    # TODO
# ------------------------ LINUX ---------------------------------
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif ()

# ------------------------ C++ SETUP ---------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------ DEPENDENCIES ------------------------------

add_subdirectory("deps")

# ------------------------ VARIABLES ---------------------------------


set(sysload_Sources
    include/sysload/sysload.h src/sysload.cpp
)

set(sysload_PublicIncludes
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(sysload_PrivateIncludes
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sysload"
)

set(sysload_Definitions)

set(sysload_Libraries
    doctest::doctest
)

# ------------------------ sysload -----------------------------

add_library(sysload
    ${sysload_Sources}
)

target_compile_definitions(sysload PRIVATE
    ${sysload_Definitions}
    DOCTEST_CONFIG_DISABLE
)

target_include_directories(sysload PUBLIC 
    ${sysload_PublicIncludes}
)

target_include_directories(sysload PRIVATE 
    ${sysload_PrivateIncludes}
)

target_link_libraries(sysload
    ${sysload_Libraries}
    ${sysload_PlatformLibs}
)

# ------------------------ sysload TESTS -----------------------

option(BUILD_TESTS "Build sysload tests" ON)

if(BUILD_TESTS)
    add_executable(sysload-tests
        test/test_main.cpp
        ${sysload_Sources}
    )

    target_compile_definitions(sysload-tests PRIVATE
        ${sysload_Definitions}
    )

    target_include_directories(sysload-tests PUBLIC
        ${sysload_PublicIncludes}
    )

    target_include_directories(sysload-tests PRIVATE 
        ${sysload_PrivateIncludes}
    )

    target_link_libraries(sysload-tests
        ${sysload_Libraries}
        ${sysload_PlatformLibs}
    )
endif()

# ------------------------ sysload EXAMPLE -----------------------

option(BUILD_EXAMPLE "Build sysload example" ON)

if(BUILD_EXAMPLE)
    add_executable(sysload-example
        example/example_main.cpp
        ${sysload_Sources}
    )

    target_compile_definitions(sysload-example PRIVATE
        ${sysload_Definitions}
    )

    target_link_libraries(sysload-example 
        sysload
    )
endif()

