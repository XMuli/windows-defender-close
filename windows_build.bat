@echo off
setlocal enabledelayedexpansion

set QT_PATH=C:\Qt\6.7.3\msvc2022_64
set BUILD_DIR=build
set BIN_DIR=bin
set PROJECT_NAME=ToggleGuardian
set "VERSION=2.2.0" 
set CONFIG_TYPE=Release 
REM true - Ninja 编译;  false - Visual Studio 17 2022 编译; 下一行行尾需加一个空格
set IS_USE_NINJA=true 
set BUILD_DIR_NAME=Desktop_Qt_6_7_3_MSVC2022_64bit-%CONFIG_TYPE% 
set "TOOL_7Z=C:\Program Files\7-Zip\7z.exe" 
REM true 额外再构建 setup 安装包且签名；false 默认不需要构建
set IS_CREATE_SETUP_PACAKE=true  

echo "****************** 构建开始 ******************"
echo Qt Path: "%QT_PATH%"
echo Build Directory: "%BUILD_DIR%"
echo Bin Directory: "%BIN_DIR%"
echo Build Type: "%CONFIG_TYPE%"

echo "****************** 正在清理旧的构建目录和 bin 文件夹 ******************"
if exist %BUILD_DIR% (
    echo 删除现有的 "%BUILD_DIR%" 文件夹...
    rmdir /s /q %BUILD_DIR%
)
if exist %BIN_DIR% (
    echo 删除现有的 "%BIN_DIR%" 文件夹...
    rmdir /s /q %BIN_DIR%
)

echo "****************** 创建新目录 ******************"
mkdir %BUILD_DIR%
mkdir %BIN_DIR%

echo "****************** 正在设置环境变量 ******************"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64
call "%QT_PATH%\bin\qtenv2.bat" 

echo "****************** 当前路径 ******************"
cd /d %~dp0
echo 当前路径: "%CD%"

set START_TIME=%TIME%

echo "****************** 选择编译方式 ******************"
if "%IS_USE_NINJA%"=="true" (
    echo "****************** 正在使用 Visual Studio 17 2022 编译 ******************"
    "C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_BUILD_TYPE=%CONFIG_TYPE% -DCMAKE_PREFIX_PATH="%QT_PATH%" ^
          -G "Visual Studio 17 2022" -A x64 . ^
          -S . -B .\%BUILD_DIR%\%BUILD_DIR_NAME%
    if %ERRORLEVEL% NEQ 0 (
        echo 配置失败！
        exit /b %ERRORLEVEL%
    )

    echo "****************** 正在编译项目 ******************"
    "C:\Program Files\CMake\bin\cmake.exe" --build .\%BUILD_DIR%\%BUILD_DIR_NAME% --target %PROJECT_NAME% -- /m:%NUMBER_OF_PROCESSORS%
    if %ERRORLEVEL% NEQ 0 (
        echo 编译失败！
        exit /b %ERRORLEVEL%
    )

) else (
    echo "****************** 正在使用 Ninja 编译 ******************"
    "C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_BUILD_TYPE=%CONFIG_TYPE% -DCMAKE_PREFIX_PATH="%QT_PATH%" ^
          -DCMAKE_MAKE_PROGRAM="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe" ^
          -G Ninja -S . -B .\%BUILD_DIR%\%BUILD_DIR_NAME%
    if %ERRORLEVEL% NEQ 0 (
        echo 配置失败！
        exit /b %ERRORLEVEL%
    )

    echo "****************** 正在编译项目 ******************"
    "C:\Program Files\CMake\bin\cmake.exe" --build .\%BUILD_DIR%\%BUILD_DIR_NAME% --target %PROJECT_NAME% -- -j%NUMBER_OF_PROCESSORS%
    if %ERRORLEVEL% NEQ 0 (
        echo 编译失败！
        exit /b %ERRORLEVEL%
    )
)

echo "****************** 编译成功 ******************"

set END_TIME=%TIME%

echo "编译开始时间: "%START_TIME%""
echo "编译结束时间: "%END_TIME%""

set /a START_HOUR=%START_TIME:~0,2%
set /a START_MINUTE=%START_TIME:~3,2%
set /a START_SECOND=%START_TIME:~6,2%

set /a END_HOUR=%END_TIME:~0,2%
set /a END_MINUTE=%END_TIME:~3,2%
set /a END_SECOND=%END_TIME:~6,2%

set /a START_TOTAL_SEC=%START_HOUR%*3600+%START_MINUTE%*60+%START_SECOND%
set /a END_TOTAL_SEC=%END_HOUR%*3600+%END_MINUTE%*60+%END_SECOND%

set /a ELAPSED_SEC=%END_TOTAL_SEC% - %START_TOTAL_SEC%
set /a ELAPSED_MINUTES=%ELAPSED_SEC% / 60
set /a ELAPSED_SECONDS=%ELAPSED_SEC% %% 60

echo "编译总耗时: "%ELAPSED_MINUTES%" 分钟 "%ELAPSED_SECONDS%" 秒"

echo "****************** 删除文件: .\bin\Qt6Pdf.dll ******************"
if exist .\bin\Qt6Pdf.dll (
    echo 删除文件: .\bin\Qt6Pdf.dll
    del /f /q .\bin\Qt6Pdf.dll
)

echo "****************** 正在为应用签名 ******************"

"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64\signtool.exe" sign /sha1 "8e383d678f5e22ddcdd42c9571d3b9a4bdbd2860" /tr http://time.certum.pl /td sha256 /fd sha256 /v ".\bin\%PROJECT_NAME%.exe"


:: 压缩文件夹
echo 压缩文件夹为 .7z 文件...
"%TOOL_7Z%" a ".\%PROJECT_NAME%_protable_%VERSION%_x64.7z" ".\%BIN_DIR%\*"


:: 如果 IS_CREATE_SETUP_PACAKE 为 true，则执行以下操作
if "%IS_CREATE_SETUP_PACAKE%"=="true" (

	echo 构建 setup exe 安装包
	"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc ".\setup_package.iss"
	
	:: 获取符合条件的 .exe 文件名（没有后缀）
	for %%f in (%PROJECT_NAME%_setup_*_x64.exe) do (
		set "FILENAME=%%~nf"
		echo 文件名（没有后缀）: !FILENAME!
	)
	
	echo 安装包的名称: !FILENAME!
    echo 重命名 "%PROJECT_NAME%_setup_%VERSION%_x64.exe"
    ren "!FILENAME!.exe" "%PROJECT_NAME%_setup_%VERSION%_x64.exe"
	
	echo "****************** signtool 软件签名 ******************"
	"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64\signtool.exe" sign /sha1 "8e383d678f5e22ddcdd42c9571d3b9a4bdbd2860" /tr http://time.certum.pl /td sha256 /fd sha256 /v ".\%PROJECT_NAME%_setup_%VERSION%_x64.exe"


) else (
    echo 未启用安装包构建和签名流程
)


REM 启动编译后的应用
REM echo "****************** 启动项目 ******************"
REM .\bin\%PROJECT_NAME%.exe

pause