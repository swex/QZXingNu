cmake_minimum_required(VERSION 2.8.12)

project(QZXingNu)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Multimedia)
find_package(Qt5Concurrent)

add_subdirectory(zxing-cpp/core)

add_library(${PROJECT_NAME} "main.cpp"
    "qzxingnu.cpp"
    "qzxingnufilter.cpp"
    )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Multimedia Qt5::Gui Qt5::Concurrent ZXingCore)
