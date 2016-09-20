cmake_minimum_required(VERSION 3.5.1)

project(sil)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (${ARCH} AND ${ARCH} EQUAL 32)
    add_definitions(-DARCH32)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if (${DEBUG} AND ${DEBUG} EQUAL 1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

file(GLOB SOURCES src/*.c)
include_directories(include)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/)
file(GLOB HEADERS include/sil/*.h)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

# Test
enable_testing()
add_subdirectory(test)
add_test(basic_getters test/basic_getters)
add_test(zero_new test/zero_new)
