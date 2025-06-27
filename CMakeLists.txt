cmake_minimum_required(VERSION 3.16)

project(focus_detector_qt 
    VERSION 1.0.0 
    DESCRIPTION "Qt6-based window focus detection and monitoring tool"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 配置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(focus-detector-qt
    main.cpp
    focusmanager.h
    focusmanager.cpp
    focuswindow.h
    focuswindow.cpp
)

# 设置目标属性
set_target_properties(focus-detector-qt PROPERTIES
    OUTPUT_NAME "focus-detector-qt"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(focus-detector-qt
    PRIVATE Qt6::Core Qt6::Widgets
)

# 安装配置
include(GNUInstallDirs)

# 安装可执行文件
install(TARGETS focus-detector-qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装文档
install(FILES 
    README.md 
    DEMO.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

# 安装演示脚本
install(FILES 
    run_demo.sh
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# CPack配置
set(CPACK_PACKAGE_NAME "focus-detector-qt")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt6-based window focus detection tool")
set(CPACK_PACKAGE_VENDOR "rewine")
set(CPACK_PACKAGE_CONTACT "rewine@qq.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/debian/copyright")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# DEB包配置
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "rewine <rewine@qq.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6widgets6, libqt6core6, libqt6gui6")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Qt6-based window focus detection and monitoring tool
 Focus Detector Qt provides real-time tracking and recording of window focus
 state changes with millisecond precision, multi-window support, and
 comprehensive logging capabilities.")

# RPM包配置
set(CPACK_RPM_PACKAGE_LICENSE "GPL-3.0")
set(CPACK_RPM_PACKAGE_GROUP "Applications/System")
set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase >= 6.0")

include(CPack)
