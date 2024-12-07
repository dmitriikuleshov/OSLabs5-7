cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable address sanitizer if needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -w")

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/tests")

include(FetchContent)

# Загружаем ZeroMQ
# FetchContent_Declare(
#   zmq
#   GIT_REPOSITORY https://github.com/zeromq/libzmq.git
#   GIT_TAG v4.3.4
# )
# FetchContent_MakeAvailable(zmq)

# Загружаем cppzmq (обертка для ZeroMQ)
# FetchContent_Declare(
#   cppzmq
#   GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
#   GIT_TAG v4.8.1
# )
# FetchContent_MakeAvailable(cppzmq)

# Include directory
include_directories(include)

# Add executables
add_executable(server ${SRC_DIR}/server.cpp)
add_executable(calc ${SRC_DIR}/calculation_node.cpp)

# Link ZeroMQ library and cppzmq to executables
target_include_directories(server PRIVATE ${cppzmq_SOURCE_DIR})
target_include_directories(calc PRIVATE ${cppzmq_SOURCE_DIR})

target_link_libraries(server PRIVATE zmq)
target_link_libraries(calc PRIVATE zmq)

# Clean target
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "server;calc")

# Find and include GTest
find_package(GTest QUIET)
if(NOT GTest_FOUND)
  # Install Google Test
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    TLS_VERIFY false
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

# Enable testing
enable_testing()

function(add_gtest_executable TEST_NAME TEST_SRC)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} GTest::GTest GTest::Main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Add the tests executable
add_gtest_executable(tests ${TEST_DIR}/tests.cpp)
