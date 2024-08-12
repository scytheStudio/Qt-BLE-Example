cmake_minimum_required(VERSION 3.16)

project(QtEsp32Demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Quick Bluetooth Multimedia)

qt_standard_project_setup()

qt_add_executable(appQtEsp32Demo
  main.cpp
)

qt_add_qml_module(appQtEsp32Demo
  URI QtEsp32Demo
  VERSION 1.0
  QML_FILES
  Main.qml
  SOURCES BLEController.h BLEController.cpp
  RESOURCES car.png
  RESOURCES sound.m4a
  RESOURCES sound_second.m4a
  RESOURCES sound_third.m4a
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQtEsp32Demo PROPERTIES
  #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQtEsp32Demo
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtEsp32Demo
  PRIVATE Qt6::Quick Qt6::Bluetooth Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appQtEsp32Demo
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
