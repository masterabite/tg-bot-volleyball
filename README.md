# WINDOWS X64 & MSBUILD

-----
vcpkg Visual Studio
1)  Установить Visual Studio;
2)  Раcположить vcpkg по пути C:\src\vcpkg;
vcpkg Git
1) cd C:\src\
3) git clone https://github.com/Microsoft/vcpkg.git
4) cd vcpkg
5) .\bootstrap-vcpkg.bat
----
4)  Установить переменную среды в Path "C:\src\vcpkg\" 
5)  связать vcpkg с Visual Studio:
    *   C:\src\vcpkg\vcpkg integrate install
6)  Установить следующие пакеты:
    *   vcpkg install boost
    *   vcpkg install curl
    *   vcpkg install cpr
    *   vcpkg install opencv
7)  [*Не актуально с версии 0.0.5*] Установка python элементов:
    *   pip install opencv-python opencv-python-headless
    *   pip install pytesseract
    <!-- *   C:\src\vcpkg\vcpkg install tgbot-cpp:x64-windows -->

8) Добавить в PATH cmake msbuild:
-   C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin
-   C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin'

9)  NVIDIA CUDA
-   скачать https://developer.nvidia.com/cuda-toolkit
-   переменная среды: C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.9\bin
11)  Сборка: bld.bat
12)  Запуск: run.bat  


<!-- 
## **CMakeLists.txt**:
1)    указать соответствующий путь для CMAKE_TOOLCHAIN_FILE (".../vcpkg/scripts/buildsystems/vcpkg.cmake"); 
-->


# LINUX & CMAKE
1)  Установка зависимостей:
    *   sudo apt install g++ make binutils cmake libboost-system-dev libssl-dev zlib1g-dev libcurl4-openssl-dev libopencv-dev curl
    *   sudo apt install python3-opencv (не актуально)
    *   sudo apt install tesseract-ocr  (не актуально)
    *   pip install pytesseract         (не актуально)

2)   cpr:
   ```Bash
sudo apt-get install meson
cd
git clone https://github.com/libcpr/cpr.git
cd cpr
rm -rf build/
mkdir build && cd build
cmake .. -DCPR_BUILD_TESTS=OFF
make && sudo make install
```

3) CUDA

После установки актуального драйвера nvidia:
```Bash
sudo apt install nvidia-cuda-toolkit
nvcc --version
```

4)  Сборка: bld.sh
5)  Запуск: run.run  
