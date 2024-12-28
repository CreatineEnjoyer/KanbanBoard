cmake_minimum_required(VERSION 3.16)

project(KanbanBoard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appKanbanBoard
    main.cpp
)

qt_add_qml_module(appKanbanBoard
    URI KanbanBoard
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES kanbanmodel.h kanbanmodel.cpp
        SOURCES taskmodel.h taskmodel.cpp
        SOURCES DatabaseSQLite.h DatabaseSQLite.cpp
        QML_FILES KanbanColumn.qml
        QML_FILES KanbanTask.qml
        SOURCES column.h column.cpp
        SOURCES IDatabaseManage.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appKanbanBoard PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appKanbanBoard
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appKanbanBoard
    PRIVATE Qt6::Core Qt6::Quick Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appKanbanBoard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
