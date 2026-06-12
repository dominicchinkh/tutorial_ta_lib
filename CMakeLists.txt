cmake_minimum_required(VERSION 4.0)

# 1. Force the C++17 standard
set(CMAKE_CXX_STANDARD 17)

# 2. Ensure that C++17 is an absolute requirement (fails early if the compiler is too old)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate TA-Lib dynamically on the system
find_library(
    TALIB_LIBRARY 
    NAMES ta-lib
    PATHS /usr/local/lib /usr/lib
)

if(NOT TALIB_LIBRARY)
    message(FATAL_ERROR "TA-Lib library not found! Did you run 'sudo make install'?")
endif()

set(
    PROJECT
    TA_LIB_EXAMPLE
)

project (${PROJECT})

# determine architecture
if(NOT ${CUSTOM_ARCH} MATCHES "x32" AND NOT ${CUSTOM_ARCH} MATCHES "x64")
    set(
        CUSTOM_ARCH
        x32
    )
endif()

# Ouput - path
# All binaries are located in the bin - folder
set(
    EXECUTABLE_OUTPUT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)
set(
    LIBRARY_OUTPUT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

include_directories(
    # project directories
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# Tell CMake where to look for TA-Lib headers and libraries
# (Adjust paths if you installed TA-Lib to a custom location)
include_directories(/usr/local/include/ta-lib)

link_directories(/usr/local/lib)

file(GLOB_RECURSE HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(
    ${PROJECT}
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT}
    ta-lib
)

add_dependencies(
    ${PROJECT}
)

set(
    SHAREDSUFFIX
    so
)
if(${CMAKE_SYSTEM} MATCHES "Darwin")
    set(
        SHAREDSUFFIX
        dylib
    )
endif()

if(${CMAKE_SYSTEM} MATCHES "Linux")
    set_target_properties(
        ${PROJECT}
        PROPERTIES
            OUTPUT_NAME 
                "test"
            LINK_FLAGS
                -Wl,-rpath,.
    )
elseif(${CMAKE_SYSTEM} MATCHES "Darwin")
    set_target_properties(
        ${PROJECT}
        PROPERTIES
            OUTPUT_NAME 
                "test"
            BUILD_WITH_INSTALL_RPATH
                1
            LINK_FLAGS
                -Wl,-rpath,@loader_path/
    )

    add_custom_command(
        TARGET
            ${PROJECT}
        WORKING_DIRECTORY
            ${EXECUTABLE_OUTPUT_PATH}
        VERBATIM
    )
endif()
