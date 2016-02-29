project(test_shaders_qml LANGUAGES CXX)
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(EXECNAME test_shaders)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/usr/jamoma/share/cmake/Jamoma/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if (OSSIA_API)
  add_subdirectory(${OSSIA_API})

  find_package(Qt5 5.3 REQUIRED COMPONENTS Core Widgets Gui Quick Qml)


  file(GLOB QML_SRC *.qml *.js)
  qt5_add_resources(RESOURCES qml.qrc)
  add_executable(${EXECNAME} main.cpp transmitter.h transmitter.cpp publisher.h publisher.cpp ${QML_SRC} ${RESOURCES})

  target_link_libraries(${EXECNAME} PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui APIJamoma Qt5::Qml)
else()
  message( SEND_ERROR "Please specify the path to the OSSIA API in the variable OSSIA_API (-DOSSIA_API=/path/to/api)" )
endif()
