cmake_minimum_required(VERSION 3.16)

project(screenlock VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(sdbus-c++)

qt_standard_project_setup()

qt_add_executable(appscreenlock
    src/main.cpp
)

qt_add_qml_module(appscreenlock
    URI screenlock
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES include/dbusmanager.h src/dbusmanager.cpp
)


target_include_directories(appscreenlock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(appscreenlock
    PRIVATE Qt6::Quick SDBusCpp::sdbus-c++
)

include(GNUInstallDirs)
install(TARGETS appscreenlock
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

