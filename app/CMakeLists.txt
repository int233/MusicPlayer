cmake_minimum_required(VERSION 3.19)
project(IntMusic LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools Multimedia Gui)

qt_standard_project_setup()

qt_add_executable(IntMusic
    WIN32 MACOSX_BUNDLE
    main.cpp
    intmusic.cpp
    intmusic.h
    intmusic.ui
    app/util/MusicPlayer/musicplayer.cpp
    app/util/MusicPlayer/musicplayer.h
)

qt_add_translations(
    TARGETS IntMusic
    TS_FILES IntMusic_zh_CN.ts
)

target_link_libraries(IntMusic
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Gui
        Qt6::Multimedia
)

include(GNUInstallDirs)

install(TARGETS IntMusic
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET IntMusic
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
