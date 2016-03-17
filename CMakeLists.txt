cmake_minimum_required(VERSION 3.0.2)

set(BINARY_NAME jougen)
set(TESTS_BINARY_NAME tests)

project(${BINARY_NAME})

set(CMAKE_CXX_EXTRA_FLAGS "-std=c++11 -Wall")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (DEBUG)
    set(CMAKE_CXX_EXTRA_FLAGS "${CMAKE_CXX_EXTRA_FLAGS} -O0 -DDEBUG=1")

    if (DEBUG_LEVEL)
        set(CMAKE_CXX_EXTRA_FLAGS "${CMAKE_CXX_EXTRA_FLAGS} -DDEBUG_LEVEL=${DEBUG_LEVEL}")
    endif()
else()
    set(CMAKE_CXX_EXTRA_FLAGS "${CMAKE_CXX_EXTRA_FLAGS} -O2 -fdata-sections -ffunction-sections")
    set(CMAKE_EXE_LINKER_EXTRA_FLAGS "${CMAKE_EXE_LINKER_EXTRA_FLAGS} -Wl,--gc-sections")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_EXTRA_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_EXTRA_FLAGS}")

set(INC_DIRS
    "inc/engine"
    "inc/gui"
    "inc/tests"
    "inc")

include_directories(${INC_DIRS})

file(GLOB ENGINE_SOURCES
    "src/engine/*.cpp")
file(GLOB GUI_SOURCES
    "src/gui/*.cpp")

file(GLOB ENGINE_HEADERS
    "inc/engine/*.hpp")
file(GLOB GUI_HEADERS
    "inc/gui/*.hpp")
file(GLOB COMMON_HEADERS
    "inc/*.hpp")

set(SOURCES
    ${ENGINE_SOURCES}
    ${GUI_SOURCES})

set(HEADERS
    ${ENGINE_HEADERS}
    ${GUI_HEADERS}
    ${COMMON_HEADERS})

set(SOURCE_FILES ${SOURCES} ${HEADERS})

find_package(Qt5Widgets)

add_executable(${BINARY_NAME} ${SOURCE_FILES} src/jougen.cpp)

target_link_libraries(${BINARY_NAME} Qt5::Widgets)

if (ENABLE_TESTS)
    file(GLOB TEST_SOURCES
        "src/tests/*.cpp")

    file(GLOB TEST_HEADERS
        "inc/tests/*.hpp")

    set(TEST_SOURCE_FILES ${SOURCE_FILES} ${TEST_SOURCES} ${TEST_HEADERS})

    add_executable(${TESTS_BINARY_NAME} ${TEST_SOURCE_FILES} src/tests.cpp)
endif()