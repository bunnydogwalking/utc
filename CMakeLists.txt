cmake_minimum_required(VERSION 3.15)
project(utc CXX)
find_package(cctz)

add_executable(${PROJECT_NAME} src/utc.C)

# Use the global target
target_link_libraries(${PROJECT_NAME} cctz::cctz)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
