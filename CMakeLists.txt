# Сборка для Windows через msbuild
if (${OS} STREQUAL "windows")
    cmake_minimum_required(VERSION 3.31)
    cmake_policy(SET CMP0167 NEW)
    set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

if (${OS} STREQUAL "linux")
    cmake_minimum_required(VERSION 3.10)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/out/${OS})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(volleyballBot)

#   функция для определения переменных
function(define_variable var_name default_value)
    if(DEFINED ${var_name})
        add_compile_definitions(${var_name}=${${var_name}})
        set(value ${${var_name}})
    else()
        add_compile_definitions(${var_name}=${default_value})
        set(value ${default_value})
    endif()
    message(STATUS "${var_name}: ${value}")
endfunction()

define_variable(BUILD_MODE 0)
define_variable(GPU_OPS 0)


if      (${OS} STREQUAL "linux")
    find_package(OpenSSL REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(usr/local/include)
    link_directories(/usr/local/lib)

    if (${GPU_OPS})
        enable_language(CUDA)
        set(CMAKE_CUDA_ARCHITECTURES 75 86)
    endif()

elseif  (${OS} STREQUAL "windows")
    find_package(cpr CONFIG REQUIRED)

    if (${GPU_OPS})
        set(CMAKE_CUDA_ARCHITECTURES 75 86)
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Wno-deprecated-gpu-targets")
        enable_language(CUDA)
    endif()

endif()

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


find_package(CURL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include/all ${CMAKE_SOURCE_DIR}/include/${OS} ${OPENSSL_INCLUDE_DIR})

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()


# Собираем все исходники в src и поддиректориях
file(GLOB_RECURSE SOURCES src/*.cpp)

# Исключаем каталоги CMake и build
list(FILTER SOURCES EXCLUDE REGEX ".*(build|CMakeFiles).*")
list(FILTER SOURCES EXCLUDE REGEX ".*test\\.cpp$")  # Исключаем тестовые файлы

# Исполняемый файл
add_executable(volleyballBot ${SOURCES})

# Добавляем все каталоги с заголовочными файлами
target_include_directories(volleyballBot PRIVATE src)

# Подключение библиотек
file(GLOB LIBS "${CMAKE_SOURCE_DIR}/lib/${OS}/*")

if      (${OS} STREQUAL "linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE /usr/local/lib/libcpr.so Threads::Threads pthread stdc++ /lib/x86_64-linux-gnu/libcurl.so.4)
elseif  (${OS} STREQUAL "windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr CURL::libcurl)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system ${LIBS} ${OPENSSL_LIBRARIES} ${OpenCV_LIBS})
