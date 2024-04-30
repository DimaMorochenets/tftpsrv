cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 5.15.3 REQUIRED COMPONENTS Core Quick SerialPort)
find_package(Qt5 COMPONENTS Core Quick SerialPort REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
            "*.cpp"
            "*.qrc"
            "*.qml"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
            "*.h"
)

add_executable(app
  ${SOURCES}
  ${HEADERS}
  src/TFTPServer.h src/TFTPServer.cpp
  src/TFTPProtocol.h src/TFTPProtocol.cpp
  src/Utilities.hpp
)

target_link_libraries(app
  Qt${QT_VERSION_MAJOR}::Core 
  Qt${QT_VERSION_MAJOR}::Quick  
  Qt${QT_VERSION_MAJOR}::SerialPort
)

set_target_properties(app PROPERTIES
  ${BUNDLE_ID_OPTION}
  WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS app
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
