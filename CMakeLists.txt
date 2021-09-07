cmake_minimum_required(VERSION 3.19)
project(QuadTree)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Math Library
include(FindPkgConfig)
find_package(MathLibrary)
include_directories(${LIBRARY_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp CGameView.cpp CGameModel.cpp CGameController.cpp)

include(cmake/AddSFML.cmake)

# LINKING LIBRARIES
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system sfml-network)
target_link_libraries(${PROJECT_NAME} ${LIBRARY})