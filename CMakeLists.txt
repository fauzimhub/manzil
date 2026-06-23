cmake_minimum_required(VERSION 3.20)

project(manzil VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME "manzil")

set(wxBUILD_SHARED OFF)
set(wxBUILD_TESTS OFF)
set(wxBUILD_SAMPLES OFF)
set(wxBUILD_DEMOS OFF)
set(JSON_BuildTests OFF)

add_subdirectory(third_party/wxwidgets)
add_subdirectory(third_party/json)

add_executable(${EXECUTABLE_NAME} src/main.cpp src/manzil.cpp)

target_include_directories(
  ${EXECUTABLE_NAME}
  SYSTEM
  PRIVATE "${CMAKE_SOURCE_DIR}/third_party/wxwidgets/include")

target_link_libraries(
  ${EXECUTABLE_NAME}
  PRIVATE wx::core wx::base nlohmann_json::nlohmann_json)

target_compile_options(
  ${EXECUTABLE_NAME}
  PRIVATE
    -g
    -pedantic
    -Wall
    -Weffc++
    -Wextra
    -Wconversion
    -Wsign-conversion)
