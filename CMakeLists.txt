cmake_minimum_required(VERSION 3.5)

project(CLCheckers2OfSoQTness VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_SOURCE_DIR}/)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        clcengine2/auxiliary/DynamicSequence.h clcengine2/auxiliary/DynamicSequence.cpp
        clcengine2/checker/CheckerMetaInfo.h
        clcengine2/movefinder/Move.h clcengine2/movefinder/Move.cpp
        clcengine2/movefinder/AbstractMoveFinder.h clcengine2/movefinder/AbstractMoveFinder.cpp
        clcengine2/movefinder/CommonMoveFinder.h clcengine2/movefinder/CommonMoveFinder.cpp
        clcengine2/movefinder/KingMoveFinder.h clcengine2/movefinder/KingMoveFinder.cpp
        clcengine2/checker/Checker.h clcengine2/checker/Checker.cpp
        clcengine2/board/BoardCell.h clcengine2/board/BoardCell.cpp
        clcengine2/board/Board.h clcengine2/board/Board.cpp
        clcengine2/analyzer/Difficulty.h
        clcengine2/analyzer/AnalyzerContainer.h
        clcengine2/analyzer/AbstractComputerRival.h clcengine2/analyzer/AbstractComputerRival.cpp
        clcengine2/analyzer/ComputerRivalMinimax.h clcengine2/analyzer/ComputerRivalMinimax.cpp
        clcengine2/analyzer/ComputerRivalRandom.h clcengine2/analyzer/ComputerRivalRandom.cpp
        makenewgamedialog.h makenewgamedialog.cpp makenewgamedialog.ui
        auxTypes/gameType.h
        auxTypes/gameparameters.h
        scenes/boardscene.h scenes/boardscene.cpp
        auxTypes/pathmapmarker.h
        auxTypes/gamestate.h
        auxTypes/gameflags.h
        auxTypes/cursormovedirections.h
        scenes/avatarscene.h scenes/avatarscene.cpp
        auxTypes/gamesavedata.h
        clcengine2/analyzer/RivalType.h

        clcengine2/analyzer/EvalTables.h
        clcengine2/analyzer/EvalTable.h clcengine2/analyzer/EvalTable.cpp
        constants.h
        mainwindow_cv.ui
        additional/Auxiliary.h additional/Auxiliary.cpp
        additional/GameState.h additional/GameState.cpp
        additional/BoardRenderer.h additional/BoardRenderer.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CLCheckers2OfSoQTness
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CLCheckers2OfSoQTness APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CLCheckers2OfSoQTness SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CLCheckers2OfSoQTness
            ${PROJECT_SOURCES}
            additional/AssetLoader.h additional/AssetLoader.cpp
            additional/PartyLogger.h additional/PartyLogger.cpp
            resources.qrc
        )

    endif()
endif()

target_link_libraries(CLCheckers2OfSoQTness PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CLCheckers2OfSoQTness)
endif()
set_target_properties(CLCheckers2OfSoQTness PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CLCheckers2OfSoQTness
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CLCheckers2OfSoQTness)
endif()
