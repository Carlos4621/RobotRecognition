cmake_minimum_required(VERSION 3.16)

project(Robot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(whisper.cpp)

set(CMAKE_PREFIX_PATH "/home/carlos4621/Qt/6.8.1/gcc_64/lib/cmake/")

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Concurrent Multimedia)
find_package(OpenCV REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui)

set(RESOURCE_FILES
    resources/hazmatClasses.txt 
    resources/hazmatModel.onnx
)

set(PROJECT_SOURCES
    main.cpp
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

qt_add_executable(Robot
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)


target_link_libraries(Robot PRIVATE Qt6::Widgets Qt6::Multimedia Qt6::Concurrent)
target_link_libraries(Robot PRIVATE ${OpenCV_LIBS} whisper)

set_target_properties(Robot PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Robot
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(Robot)