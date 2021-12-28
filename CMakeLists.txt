cmake_minimum_required(VERSION 2.8.12)

project(QZXingNu)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Multimedia Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Multimedia Concurrent)



add_subdirectory(zxing-cpp/core)

add_library(${PROJECT_NAME}
    "qzxingnu.cpp"
    "qzxingnu.h"
    "qzxingnufilter.cpp"
    "qzxingnufilter.h"
    "QZXingNu"
    "QZXingNuFilter"
    )

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Concurrent
    ZXing)

set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}_example "example/main.cpp" "example/qml.qrc")
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Quick)
