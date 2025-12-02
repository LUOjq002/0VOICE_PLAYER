cmake_minimum_required(VERSION 3.10)
project(0voice_player C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "/home/ljq/software/Qt/6.9.3/gcc_64" ${CMAKE_PREFIX_PATH})

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets) 
find_package(SDL2 REQUIRED)

set(FFMPEG_INSTALL_ROOT "/usr/local/ffmpeg")
set(ENV{PKG_CONFIG_PATH} "${FFMPEG_INSTALL_ROOT}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavdevice
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
    libavfilter
)

file(GLOB SOURCE_FILES "Sources/*.cpp" "Sources/*.c")
add_executable(0voice_player ${SOURCE_FILES})

# 添加头文件目录
target_include_directories(0voice_player PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    # ${FFMPEG_INCLUDE_DIRS}
)

target_link_libraries(0voice_player PRIVATE PkgConfig::FFMPEG)
target_link_libraries(0voice_player PRIVATE Qt::Core
                                          Qt::Gui
                                          Qt::Widgets)
target_link_libraries(0voice_player PRIVATE SDL2::SDL2) 

# 关键：对“目标”设置 RPATH（构建期 & 安装后）
set(FFMPEG_LIB_DIR "${FFMPEG_INSTALL_ROOT}/lib")
set_target_properties(0voice_player PROPERTIES
  BUILD_RPATH "${FFMPEG_LIB_DIR}"
  INSTALL_RPATH "${FFMPEG_LIB_DIR}"
  SKIP_BUILD_RPATH FALSE
  BUILD_WITH_INSTALL_RPATH FALSE
)


# 保险起见，再加一条链接器 rpath（有的系统更吃这一口）
target_link_options(0voice_player PRIVATE -Wl,-rpath,${FFMPEG_LIB_DIR})