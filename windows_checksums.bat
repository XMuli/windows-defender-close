@echo off
setlocal enabledelayedexpansion

:: 配置输出文件路径
set "OUTPUT_FOLDER=.\Output_3.1.3"
set "OUTPUT_FILE=%OUTPUT_FOLDER%\checksums_md5_sha256.txt"


:: 检查并清空输出文件（如果已存在）
if exist "%OUTPUT_FILE%" del /f /q "%OUTPUT_FILE%"

:: 遍历指定文件计算校验值
for /r "%OUTPUT_FOLDER%" %%f in (*) do (
    echo 处理文件: %%f

    :: 初始化哈希值
    set "MD5="
    set "SHA1="
    set "SHA256="

    :: 获取并输出 MD5 校验值，去除多余的提示信息，只保留哈希值
    for /f "skip=1 tokens=*" %%a in ('certutil -hashfile "%%f" MD5 ^| findstr /v /c:"CertUtil:"') do (
        set "MD5=%%a"
    )
    set "MD5=!MD5: =!"

    :: 获取并输出 SHA1 校验值，去除多余的提示信息，只保留哈希值
    for /f "skip=1 tokens=*" %%a in ('certutil -hashfile "%%f" SHA1 ^| findstr /v /c:"CertUtil:"') do (
        set "SHA1=%%a"
    )
    set "SHA1=!SHA1: =!"

    :: 获取并输出 SHA256 校验值，去除多余的提示信息，只保留哈希值
    for /f "skip=1 tokens=*" %%a in ('certutil -hashfile "%%f" SHA256 ^| findstr /v /c:"CertUtil:"') do (
        set "SHA256=%%a"
    )
    set "SHA256=!SHA256: =!"

    :: 输出计算出来的校验值到文件，使用制表符进行对齐
    echo %%~nxf >> "%OUTPUT_FILE%"
    echo MD5:     !MD5! >> "%OUTPUT_FILE%"
    echo SHA-1:   !SHA1! >> "%OUTPUT_FILE%"
    echo SHA-256: !SHA256! >> "%OUTPUT_FILE%"
    echo. >> "%OUTPUT_FILE%"
)

echo 校验值生成完成! 校验信息已写入到 %OUTPUT_FILE%

:: pause