cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()
project(manzil VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME "manzil")

find_program(ODIN_EXECUTABLE odin DOC "Path to the Odin compiler")

if(NOT ODIN_EXECUTABLE)
  message(
    FATAL_ERROR
      "odin executable not found. Please install Odin and ensure it is on your PATH.\n"
      "See: https://odin-lang.org/docs/install/")
endif()

message(STATUS "Found odin: ${ODIN_EXECUTABLE}")

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src/generated)
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/src/generated/quranite_data.hpp
  COMMAND
    odin
    run
    ${CMAKE_SOURCE_DIR}/src/embed_json.odin
    -file
    --
    ${CMAKE_SOURCE_DIR}/assets/chapters-data.json
    ${CMAKE_SOURCE_DIR}/assets/verses_ar_original.json
    ${CMAKE_SOURCE_DIR}/assets/verses_en_sam-gerrans_with-notes.json
    ${CMAKE_SOURCE_DIR}/assets/notes_en_sam-gerrans.json
    ${CMAKE_SOURCE_DIR}/src/generated/quranite_data.hpp
  DEPENDS
    ${CMAKE_SOURCE_DIR}/assets/chapters-data.json
    ${CMAKE_SOURCE_DIR}/assets/verses_ar_original.json
    ${CMAKE_SOURCE_DIR}/assets/verses_en_sam-gerrans_with-notes.json
    ${CMAKE_SOURCE_DIR}/assets/notes_en_sam-gerrans.json)

add_custom_target(
  generate_json_headers
  DEPENDS ${CMAKE_SOURCE_DIR}/src/generated/quranite_data.hpp)

set(wxBUILD_SHARED OFF)
set(wxBUILD_TESTS OFF)
set(wxBUILD_SAMPLES OFF)
set(wxBUILD_DEMOS OFF)

set(wxUSE_REGEX OFF CACHE BOOL "" FORCE)
set(wxUSE_NOTIFICATION_MESSAGE OFF CACHE BOOL "" FORCE)
set(wxUSE_ZLIB OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBJPEG OFF CACHE BOOL "" FORCE)
set(wxUSE_ARTPROVIDER_TANGO OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBPNG OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBTIFF OFF CACHE BOOL "" FORCE)
set(wxUSE_SVG OFF CACHE BOOL "" FORCE)
set(wxUSE_NANOSVG OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBWEBP OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBSDL OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBLZMA OFF CACHE BOOL "" FORCE)
set(wxUSE_LIBMSPACK OFF CACHE BOOL "" FORCE)

set(JSON_BuildTests OFF)

set(wxUSE_EXPAT builtin CACHE STRING "" FORCE)
set(wxUSE_WEBVIEW ON)
if(NOT WIN32)
  set(wxUSE_WEBVIEW_WEBKIT ON)
endif()

add_compile_options(-Wno-error=date-time)

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
  src/components/header_card.cpp
  src/components/reader.cpp
  src/components/reader_dialog.cpp
  src/events.cpp)

add_dependencies(${EXECUTABLE_NAME} generate_json_headers)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  enable_language(RC)
  target_sources(${EXECUTABLE_NAME} PRIVATE src/manzil.rc)
endif()

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
    $<$<CONFIG:Debug>:-g -pedantic -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Wno-enum-enum-conversion>
    $<$<CONFIG:Release>:-Os -fvisibility=hidden -fno-ident -DNDEBUG -ffunction-sections -fdata-sections>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-flto=auto>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>,$<NOT:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>>>:-flto=thin>)

target_link_options(
  ${EXECUTABLE_NAME}
  PRIVATE
    $<$<CONFIG:Release>:-s -Wl,--gc-sections>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-flto=auto>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>,$<NOT:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>>>:-flto=thin>
    $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:-Wl,--subsystem,windows>)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_BINARY_DIR}/third_party/wxwidgets/lib/WebView2Loader.dll"
      "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>"
    COMMENT "Copying WebView2Loader.dll next to ${EXECUTABLE_NAME}"
    VERBATIM)
endif()

target_compile_definitions(
  ${EXECUTABLE_NAME}
  PRIVATE $<$<CONFIG:Debug>:DEBUG_MODE=1> $<$<CONFIG:Release>:RELEASE_MODE=1>)

