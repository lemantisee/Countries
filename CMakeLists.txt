cmake_minimum_required(VERSION 3.20)

project(Countries VERSION 0.1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(BINDIR "${CMAKE_CURRENT_BINARY_DIR}/bin")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.h MainWindow.cpp
    Database.h Database.cpp

    sqlite/sqlite3.h sqlite/sqlite3.c

    TreeView/TreeView.h TreeView/TreeView.cpp
    TreeView/TreeViewItem.h TreeView/TreeViewItem.cpp
    TreeView/TreeViewModel.h TreeView/TreeViewModel.cpp
    )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Widgets)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeView
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
