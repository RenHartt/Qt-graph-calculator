cmake_minimum_required(VERSION 3.16)
project(Calculator)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)

include_directories(${CMAKE_SOURCE_DIR}/inc)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/inc/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::PrintSupport)
