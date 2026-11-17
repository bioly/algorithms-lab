cmake_minimum_required(VERSION 3.24)  # 3.24 is needed for FetchContent's FIND_PACKAGE_ARGS

project(algorithms_lab
        VERSION 0.1.0
        DESCRIPTION "A learning spot for tricky algorithm problems, in modern C++"
        LANGUAGES CXX)

# ---- Language settings (applied to every target through algolab_options) ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)               # -std=c++20, not gnu++20: stay portable
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)       # lets clangd / IDEs understand the code

# ---- Options ----
option(ALGOLAB_BUILD_TRACES "Build the step-by-step trace demos" ON)
option(ALGOLAB_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# One interface target carries the warning flags, so every challenge gets them
# without repeating them.
add_library(algolab_options INTERFACE)
if(MSVC)
  target_compile_options(algolab_options INTERFACE /W4)
else()
  target_compile_options(algolab_options INTERFACE -Wall -Wextra -Wpedantic -Wshadow)
endif()
if(ALGOLAB_WARNINGS_AS_ERRORS)
  target_compile_options(algolab_options INTERFACE
                         $<IF:$<CXX_COMPILER_ID:MSVC>,/WX,-Werror>)
endif()

include(cmake/GoogleTestSetup.cmake)
include(cmake/AddChallenge.cmake)

enable_testing()
add_subdirectory(challenges)
