cmake_minimum_required(VERSION 3.14)
project(chip8)

IF (WIN32)
    # set stuff for windows
    # windows specific stuff
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    set(SDL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL")
ELSE()
  # set stuff for other systems
ENDIF()


add_executable(${PROJECT_NAME} main.cpp chip8.h chip8.cpp sdl_helpers.hpp)

IF (WIN32)
    # vs specific helper
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    if(NOT TARGET SDL2)
        add_subdirectory("${SDL_DIR}" ${PROJECT_BINARY_DIR}/SDL2)
        target_link_libraries(${PROJECT_NAME} SDL2)
        target_include_directories(${PROJECT_NAME} PUBLIC "${SDL_DIR}/include")
    endif()
ELSE()
  # set stuff for other systems
ENDIF()

