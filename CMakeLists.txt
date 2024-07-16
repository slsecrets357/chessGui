cmake_minimum_required(VERSION 3.16)

project(Chess2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appChess2
    src/main.cpp
)

qt_add_qml_module(appChess2
    URI Chess2
    VERSION 1.0
    QML_FILES
        Main.qml
        ui/Board.qml
        ui/UserProfile.qml
        SOURCES src/Position.h
        SOURCES src/Board.h src/Board.cpp
        SOURCES src/Game.h src/Game.cpp
        SOURCES src/Utility.h
        SOURCES src/Move.h src/Move.cpp
        SOURCES src/Piece.h src/Piece.cpp
        SOURCES src/GameInterface.h src/GameInterface.cpp
        SOURCES src/Console.h src/Console.cpp
        SOURCES src/StockfishWrapper.h
    RESOURCES
        "ui/Images/bp.png"
        "ui/Images/br.png"
        "ui/Images/bn.png"
        "ui/Images/bb.png"
        "ui/Images/bk.png"
        "ui/Images/bq.png"
        "ui/Images/wp.png"
        "ui/Images/wr.png"
        "ui/Images/wn.png"
        "ui/Images/wb.png"
        "ui/Images/wk.png"
        "ui/Images/wq.png"
        "ui/Images/ff.png"
        "ui/Images/player1.png"
        "ui/Images/player2.png"
        "ui/Images/ai.jpeg"
        "ui/Images/xinlinnn.png"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appChess2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appChess2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appChess2
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appChess2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
