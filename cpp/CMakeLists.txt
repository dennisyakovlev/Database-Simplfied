cmake_minimum_required(VERSION 3.18.4)

set (CMAKE_CXX_STANDARD 17)

project(PROB)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(database)

target_include_directories(${PROJECT_NAME} PUBLIC database)

target_link_directories(${PROJECT_NAME} PRIVATE databse)

target_link_libraries(${PROJECT_NAME} prob)