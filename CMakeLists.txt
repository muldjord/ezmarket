cmake_minimum_required(VERSION 3.16)

project(EZMarket VERSION 1.1.0 LANGUAGES CXX)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DVERSION=${VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
  set(APP_ICON_RESOURCE "win32_icon.rc")
endif()

# Set build type with '-DCMAKE_BUILD_TYPE=type' where type is usually 'Debug' or 'Release'.
# Default to 'Release' type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/debug CACHE PATH "Install path prefix" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/release CACHE PATH "Install path prefix" FORCE)
  endif()
endif()

find_package(SFML 2 REQUIRED COMPONENTS audio)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia LinguistTools)

qt_standard_project_setup()

qt_add_resources(QT_RESOURCES
  ezmarket.qrc
)

set(SOURCES
  src/main.cpp
  src/mainwindow.cpp
  src/imgtools.cpp
  src/lineedit.cpp
  src/spinbox.cpp
  src/buttongroup.cpp
  src/entryeditor.cpp
  src/accountstab.cpp
  src/accountsmodel.cpp
  src/accounteditor.cpp
  src/accountwidget.cpp
  src/categoriestab.cpp
  src/categoriesmodel.cpp
  src/categoryeditor.cpp
  src/categorywidget.cpp
  src/itemstab.cpp
  src/itemsmodel.cpp
  src/itemeditor.cpp
  src/itemwidget.cpp
  src/soundboard.cpp
  src/soundmixer.cpp
  src/checkout.cpp
  src/aboutbox.cpp
)

qt_add_executable(EZMarket
  ${SOURCES}
  ${QT_RESOURCES}
  ${APP_ICON_RESOURCE}
)

target_link_libraries(EZMarket PRIVATE Qt6::Widgets Qt6::Multimedia sfml-audio)

set_target_properties(EZMarket PROPERTIES
  COMPILE_WARNING_AS_ERROR ON
  WIN32_EXECUTABLE ON
)

# Update translation from sources:
# - Use 'make update_translations' to update ts file
# - Use 'make release_translations' to update qm file
qt_add_translations(EZMarket TS_FILES ezmarket_da_DK.ts
  SOURCES ${SOURCES}
  # LUPDATE_OPTIONS -no-obsolete
)

add_dependencies(EZMarket release_translations)

install(TARGETS EZMarket
  RUNTIME DESTINATION .
)

install(FILES
  ${CMAKE_SOURCE_DIR}/AUTHORS
  ${CMAKE_SOURCE_DIR}/LICENSE
  ${CMAKE_CURRENT_BINARY_DIR}/ezmarket_da_DK.qm
  ${CMAKE_SOURCE_DIR}/README.md
  DESTINATION .
)

install(DIRECTORY
  ${CMAKE_SOURCE_DIR}/barcodes
  ${CMAKE_SOURCE_DIR}/graphics
  ${CMAKE_SOURCE_DIR}/sounds
  DESTINATION .
)
