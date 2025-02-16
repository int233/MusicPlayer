cmake_minimum_required(VERSION 3.21)
project(IntMusic VERSION 0.1 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/mingw_64/lib/cmake/Qt6")

# 查找Qt组件
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    Sql
    LinguistTools
)

qt_standard_project_setup()


# 包含当前目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 配置TagLib构建
include(ExternalProject)
set(TAGLIB_VERSION 2.0.2)
set(TAGLIB_INSTALL_DIR ${CMAKE_BINARY_DIR}/taglib-install)

ExternalProject_Add(
    taglib
    GIT_REPOSITORY "https://github.com/taglib/taglib.git"
    GIT_TAG "v${TAGLIB_VERSION}"
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL_DIR}
        -DWITH_ZLIB=OFF
        -DWITH_MP4=ON
        -DBUILD_TESTING=OFF
        -DBUILD_SHARED_LIBS=OFF
    BUILD_BYPRODUCTS 
        ${TAGLIB_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tag${CMAKE_STATIC_LIBRARY_SUFFIX}
)

# 添加可执行文件
add_executable(IntMusic
    app/custom/CardButton/cardbutton.cpp
    app/custom/CardCombo/cardcombo.cpp
    app/custom/CardPathSelect/cardpathselect.cpp
    app/custom/FlexibleTabWidget/flexibletabwidget.cpp
    app/custom/MainSubTitle/mainsubtitle.cpp
    app/util/Config/configmanager.cpp
    app/util/Config/musiclibrary.cpp
    app/util/MusicDB/musicscanner.cpp
    app/util/MusicPlayer/musicplayer.cpp
    app/view/IntMusicIndexTab/index.cpp
    app/view/IntMusicIndexTab/intmusicindextab.cpp
    app/view/IntMusicIndexTab/musiclist.cpp
    app/view/IntMusicIndexTab/setting.cpp
    app/view/MiniPlayer/miniplayer.cpp
    app/view/MusicListView/musiclistview.cpp
    app/view/Setting/settingcommon.cpp
    app/view/Setting/settingconfig.cpp
    app/main.cpp
    app/intmusic.cpp
)

qt_add_translations(
    TARGETS IntMusic
    TS_FILES IntMusic_zh_CN.ts
)

# 包含目录
target_include_directories(IntMusic PRIVATE
    ${TAGLIB_INSTALL_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/app
)

target_compile_definitions(IntMusic PRIVATE 
    TAGLIB_STATIC  # 使用静态库时必须定义
)

# 链接库
target_link_libraries(IntMusic PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Sql
    ${TAGLIB_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tag${CMAKE_STATIC_LIBRARY_SUFFIX}
)

include(GNUInstallDirs)

install(TARGETS IntMusic
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 添加依赖关系
add_dependencies(IntMusic taglib)

set(deploy_tool_options_arg "")
if(APPLE)
    set(deploy_tool_options_arg --hardened-runtime)
elseif(WIN32)
    set(deploy_tool_options_arg --no-compiler-runtime)
endif()
qt_generate_deploy_app_script(
    TARGET IntMusic
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_TOOL_OPTIONS ${deploy_tool_options_arg}
)

install(SCRIPT ${deploy_script})