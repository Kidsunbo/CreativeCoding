cmake_minimum_required(VERSION 3.14)
project(RayCasting)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(SFML)

add_definitions(-DSFML_STATIC)

set(CMAKE_EXE_LINKER_FLAGS -mwindows ${CMAKE_EXE_LINKER_FLAGS})

add_executable(RayCasting RayCasting.cpp)

target_include_directories(RayCasting PRIVATE SFML/include)
target_link_libraries(RayCasting PRIVATE sfml-window sfml-graphics sfml-system)

add_executable(BeatifulBubbles BeatifulBubbles.cpp)

target_include_directories(BeatifulBubbles PRIVATE SFML/include)
target_link_libraries(BeatifulBubbles PRIVATE sfml-window sfml-graphics sfml-system)

