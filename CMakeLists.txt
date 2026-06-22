cmake_minimum_required(VERSION 3.20)

project(manzil)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# wxWidgets options - set before add_subdirectory
set(wxBUILD_SHARED OFF) # static linking
set(wxBUILD_TESTS OFF)
set(wxBUILD_SAMPLES OFF)
set(wxBUILD_DEMOS OFF)

add_subdirectory(third_party/wxwidgets)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE wx::core wx::base)

target_include_directories(
  ${PROJECT_NAME}
  SYSTEM
  PRIVATE "${CMAKE_SOURCE_DIR}/third_party/wxwidgets/include")

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
    -g
    -pedantic
    -Wall
    -Weffc++
    -Wextra
    -Wconversion
    -Wsign-conversion)
