cmake_minimum_required(VERSION 3.20)

project(manzil VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME "manzil")

set(wxBUILD_SHARED OFF)
set(wxBUILD_TESTS OFF)
set(wxBUILD_SAMPLES OFF)
set(wxBUILD_DEMOS OFF)
set(JSON_BuildTests OFF)
set(wxUSE_WEBVIEW ON)
set(wxUSE_WEBVIEW_WEBKIT ON)

add_subdirectory(third_party/wxwidgets)
add_subdirectory(third_party/json)

add_executable(
  ${EXECUTABLE_NAME}
  src/main.cpp
  src/manzil.cpp
  src/components/quranite.cpp
  src/components/frame.cpp
  src/components/about_dialog.cpp
  src/components/surah_card.cpp
  src/components/reader.cpp
  src/components/reader_dialog.cpp
  src/events.cpp)

target_include_directories(
  ${EXECUTABLE_NAME}
  SYSTEM
  PRIVATE "${CMAKE_SOURCE_DIR}/third_party/wxwidgets/include")

target_link_libraries(
  ${EXECUTABLE_NAME}
  PRIVATE wx::core wx::base wx::webview nlohmann_json::nlohmann_json)

target_compile_definitions(
  ${EXECUTABLE_NAME}
  PRIVATE APP_VERSION="${PROJECT_VERSION}")

target_compile_options(
  ${EXECUTABLE_NAME}
  PRIVATE
    -g
    -pedantic
    -Wall
    -Weffc++
    -Wextra
    -Wconversion
    -Wsign-conversion
    -Wno-enum-enum-conversion)
