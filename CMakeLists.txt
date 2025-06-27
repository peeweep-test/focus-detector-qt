cmake_minimum_required(VERSION 3.16)

project(focus_detector_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(appfocus_detector_qt
    main.cpp
    focusmanager.h
    focusmanager.cpp
    focuswindow.h
    focuswindow.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfocus_detector_qt PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfocus_detector_qt
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfocus_detector_qt
    PRIVATE Qt6::Core Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appfocus_detector_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
