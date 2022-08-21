cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME DWCtrl)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

if(MSVC)
    OPTION(USE_MP "use multiple" ON)
    OPTION(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP
            "Set The Global Option COMPILE_FLAGS /MP to target." ON)
    if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif()

    set(VS_STARTUP_PROJECT ${PROJECT_NAME})
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        dwctrl.h
        dwctrl.cpp
        mainui.h
        mainui.cpp
        mainui.ui
)

add_executable(DWCtrl ${PROJECT_SOURCES} )

target_link_libraries(DWCtrl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(DWCtrl PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32_LEAN_AND_MEAN                            # 仅仅包含常用的 API 的头文件，加快编译速度，效果明显
        _CRT_SECURE_NO_WARNINGS                        # 允许使用 strcpy，scanf等不安全的函数
        UNICODE
        _UNICODE)
endif()
