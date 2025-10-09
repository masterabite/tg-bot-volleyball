set OS=windows
set mode=0
set saveMake=0

chcp 65001

:loop_keys
    if "%1"=="" goto end

    if "%1"=="--mode" (
        if "%2"=="dev" (
            set mode=0
        ) else if "%2"=="deb" (
            set mode=1
        ) else if "%2"=="prod" (
            set mode=2
        ) else (
            echo Неизвестный аргумент: %2
            exit /b 1
        )
        shift
    ) else  if "%1"=="--savemake" (
        set saveMake=1
    ) else (
        echo Неизвестный ключ: %1
        exit /b 1
    )
    shift
    goto loop_keys
:end

echo Установленные параметры:
echo    OS = %OS%
echo    mode = %mode%
echo    saveMake = %saveMake%


mkdir build\%OS%
mkdir out\%OS%
cd build\%OS%

if %saveMake%==0 (
    rm -rf ./*
)
set VCPKG_VISUAL_STUDIO_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\Community

cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DOS=%OS% -DBUILD_MODE=%mode% ../..
msbuild volleyballBot.sln