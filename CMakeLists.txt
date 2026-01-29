cmake_minimum_required(VERSION 3.15)
project(visualizer)

include(FetchContent)

# Set C++ standard (SFML 3.0 requires at least C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_STATIC_LIBRARIES ON)

# Also explicitly export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Audio)

set(KISSFFT_DATATYPE "int16_t" CACHE STRING "Data type for kissfft")
FetchContent_Declare(
  kissfft
  GIT_REPOSITORY https://github.com/mborgerding/kissfft.git
  GIT_TAG 131.2.0  # Use specific tag
)

set(KISSFFT_TEST OFF CACHE BOOL "Disable kissfft tests" FORCE)
FetchContent_MakeAvailable(kissfft)

# Auto-find all source files (modern approach)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/**/*.cpp" "src/*.cpp" "src/**/*.c" "src/*.c")

# Verify sources were found
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in src/ directory!")
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries (must come AFTER add_executable)
target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    kissfft
)

