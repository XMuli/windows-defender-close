# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2022-2024 XMuli
# SPDX-GitHub: https://github.com/XMuli/windows-defender-close
# SPDX-Author: XMuli <xmulitech@gmail.com>

cmake_minimum_required(VERSION 3.5)

set(project_name Windows-defender-close)
set(project_version_major 1)
set(project_version_minor 1)
set(project_version ${project_version_major}.${project_version_minor})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin") # qtcreator + msvc 可执行文件的输出目录
set(CMAKE_BINARY_DIR "${EXECUTABLE_OUTPUT_PATH}")             # visual studio 2022 可执行的输出目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})                      # 创建输出目录

# 作为宏传递给 C++ 代码
add_definitions(-DMY_PROJECT_NAME="${project_name}")
add_definitions(-DMY_BUILD_TIME="2024-01-27")
add_definitions(-DMY_VARIABLE="${project_version}")

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

set(qt_moduls Widgets)
set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
    message("--->qt_moduls:" Qt${QT_VERSION_MAJOR}::${it})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link


set(PROJECT_SOURCES
    main.cpp
    dwctrl.h
    dwctrl.cpp
    mainui.h
    mainui.cpp
    mainui.ui
    resources/logo/resources.rc
)

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false
    "${src_root_path}/*.cpp"
    "${src_root_path}/*.h"
    "${src_root_path}/*.ui")
source_group(TREE ${src_root_path} FILES ${source_lists})                                   # will be organized according to the actual directory structure, .h.cpp is put together

#***************************************************通用参数的定义 开始**********************************************
# get kit architecture bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(architecture_bit "x86")
    set(build_x64bit false)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(architecture_bit "x64")
    set(build_x64bit true)
endif()

# 设置变量 qt/ssl/lupdate/lrelease 在不同编译器下的环境和版本下的 path
# env_qt_version_dir/env_qt_dir/lupdate_path/lrelease_path;  [exe_dir] <->  EXECUTABLE_OUTPUT_PATH | CMAKE_BINARY_DIR
if(WIN32)
    set(env_qt_version_dir "C:/Qt/${QT_VERSION}")
    if(${build_x64bit})
        set(env_os "Windows(kits 64bit)")
        if(MSVC)
            set(env_qt_dir "${env_qt_version_dir}/msvc2019_64")
        elseif(MINGW)
            set(env_qt_dir "${env_qt_version_dir}/mingw81_64")
        endif()
    else()
        set(env_os "Windows(kits 86bit)")
        if(MSVC)
            set(env_qt_dir "${env_qt_version_dir}/msvc2019")
        elseif(MINGW)
            set(env_qt_dir "${env_qt_version_dir}/mingw81_32")
        endif()
    endif()

    execute_process(COMMAND cmd.exe "/C" "echo %USERPROFILE%" OUTPUT_VARIABLE HOME_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

set(lupdate_path "${env_qt_dir}/bin/lupdate")
set(lrelease_path "${env_qt_dir}/bin/lrelease")

message("--->architecture_bit:" ${architecture_bit}  "  build_x64bit:" ${build_x64bit}  "  ssl_dir:" ${ssl_dir} "  HOME_DIR:" ${HOME_DIR} "  env_os:" ${env_os})
message("lupdate_path:" ${lupdate_path})
message("lrelease_path:" ${lrelease_path})
message("EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH})
message("CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR})

#***************************************************拷贝资源文件到 bin 下**********************************************
# 拷贝配置文件
#configure_file(xconfig.ini ${exe_dir}/xconfig.ini COPYONLY)

# copy "resources" 文件夹下的指定文件夹
set(folders_to_copy "licenses" "logo")
set(source_root "${CMAKE_SOURCE_DIR}/resources")
set(traget_root "${CMAKE_BINARY_DIR}/resources")

# 遍历复制文件夹集合中的每个文件夹
foreach(it ${folders_to_copy})
    set(copy_source_dir "${source_root}/${it}")
#    set(copy_traget_dir "${traget_root}/${it}")
    file(COPY ${copy_source_dir} DESTINATION ${traget_root})
endforeach()
#************************************************* .ts 生成 .qm***************************************************
# 创建保存 ts 的文件夹，和依据里面的 *.ts 文件，生成 .qm 放入最终文件中
set(ts_dir "${CMAKE_CURRENT_SOURCE_DIR}/translations")
if(APPLE)
    set(qm_dir ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app/Contents/MacOS/translations)
else()
    set(qm_dir ${EXECUTABLE_OUTPUT_PATH}/translations)
endif()

file(MAKE_DIRECTORY "${ts_dir}")
file(MAKE_DIRECTORY "${qm_dir}")
file(GLOB ts_files "${ts_dir}/*.ts")

set(ts_files_name "")    # 获取文件名部分（不包括路径）
foreach(it ${ts_files})
    get_filename_component(ts_filename ${it} NAME_WE)
    list(APPEND ts_files_name ${ts_filename})
endforeach()

##编译时更新 .ts 和生成 .qm
foreach(it ${ts_files_name})
    # 命令参数问题：确保命令的参数传递方式正确。有时候在使用 execute_process 时，参数的传递需要一些特殊的格式，比如将参数放到列表中传递。
    execute_process(COMMAND "${lupdate_path}" -no-obsolete -no-ui-lines "${CMAKE_SOURCE_DIR}" -ts "${ts_dir}/${it}.ts") # 注意参数  # -no-obsolete -no-ui-lines
    execute_process(COMMAND "${lrelease_path}" "${ts_dir}/${it}.ts" -qm "${qm_dir}/${it}.qm")
endforeach()

file(GLOB_RECURSE qm_files "${qm_dir}/*.qm")

message("-->ts_files_name: ${ts_files_name}")
message("-->ts_files: ${ts_files}")
message("-->qm_files: ${qm_files}")

#***************************************************.ts 生成 .qm 结束**********************************************

add_executable(${project_name} ${PROJECT_SOURCES})
target_link_libraries(${project_name} PRIVATE ${qt_version_moduls})

set_target_properties(${project_name} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 获取 CPU 核心数量，开启 IDE 多核编译
include(ProcessorCount)
ProcessorCount(N)
message("-->Number of available processor cores: ${N}")

if(WIN32)
    if(MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    elseif(MINGW)
        set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${N}")
    endif()
elseif(UNIX)
    add_compile_options(-j${N})
endif()

if(MSVC)
    # 设置默认启动 project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})
    # Fix: Visual Studio 2022 生成的目标文件的输出目录, 移除掉 Release/Debug 的多一层文件夹
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}")
    # 开启 MSVC 终端上的中文不乱码
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    # 开启 UNICODE 等常规
    target_compile_definitions(${project_name} PRIVATE
        WIN32_LEAN_AND_MEAN                                                                  # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed
        UNICODE
        _UNICODE)
endif()

# Define some variables that can be used in *.cpp
target_compile_definitions(${project_name} PUBLIC _PROJECT_VERSION="${project_version}")
target_compile_definitions(${project_name} PUBLIC _BIT_ARCH=${CMAKE_SIZEOF_VOID_P})          # 4/8
target_compile_definitions(${project_name} PUBLIC _COMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown  Win: AMD64
target_compile_definitions(${project_name} PUBLIC _COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC or GNU

message("------------BRGIN------------")
message("_PROJECT_NAME:${project_name}")
message("_PROJECT_VERSION:" ${project_version})
message("_BIT_ARCH:" ${CMAKE_SIZEOF_VOID_P})
message("_COMPILER:" ${CMAKE_HOST_SYSTEM_PROCESSOR})
message("_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message("CMake Path:"
    "\n PROJECT_NAME:"  ${PROJECT_NAME}
    "\n CMAKE_INSTALL_PREFIX: "  ${CMAKE_INSTALL_PREFIX}
    "\n PROJECT_BINARY_DIR: "  ${PROJECT_BINARY_DIR}
    "\n CMAKE_INSTALL_BINDIR: "  ${CMAKE_INSTALL_BINDIR}
    "\n CMAKE_INSTALL_LIBDIR: "  ${CMAKE_INSTALL_LIBDIR}
    "\n CMAKE_INSTALL_INCLUDEDIR: "  ${CMAKE_INSTALL_INCLUDEDIR})
message("------------END------------")

install(TARGETS ${project_name}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${project_name})
endif()


#打包和压缩***************************************************************************************
# 拷贝对应的 qt 依赖

if(WIN32)
    set(windeployqt "${env_qt_dir}/bin/windeployqt.exe")
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND "${windeployqt}" "${EXECUTABLE_OUTPUT_PATH}/${project_name}.exe" --no-translations
    )
endif()

message("--->windeployqt(or mac/linux): ${windeployqt}")

# 添加一个自定义的目标，将编译和自定义打包命令关联起来
add_custom_target(run_custom_qt_packaging
    DEPENDS ${project_name}
)

