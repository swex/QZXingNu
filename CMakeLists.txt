cmake_minimum_required(VERSION 2.8.12)

project(QZXingNu)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Multimedia)
find_package(Qt5Concurrent)

add_subdirectory(zxing-cpp/core)

add_library(${PROJECT_NAME}
    "qzxingnu.cpp"
    "qzxingnufilter.cpp"
    )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Multimedia Qt5::Gui Qt5::Concurrent ZXingCore)

set(CMAKE_AUTORCC ON)
find_package(Qt5Quick)

add_executable(${PROJECT_NAME}_example "example/main.cpp" "example/qml.qrc")
target_link_libraries(${PROJECT_NAME}_example Qt5::Core Qt5::Multimedia Qt5::Gui Qt5::Concurrent Qt5::Quick ${PROJECT_NAME})
