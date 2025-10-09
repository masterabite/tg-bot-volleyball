OS="linux"
mkdir -p build/$OS
mkdir -p out/$OS
cd build/$OS

buildMode=0
saveMake=0
gpuOps=0

# Функция для вывода справки
show_help() {
    echo 
    echo "Скрипт $0 используется для сборки проекта volleyballBot"
    echo "__________________________________________________________________________________________________________________________"
    echo "Опции:"
    echo "  -m|--mode       <режим>         Установить режим сборки. Допустимые значения: 'dev', 'deb', 'prod' (по умолчанию 'prod')"
    echo "  -s|--savemake                   Не обновлять make файлы для сборки (устанавливает save_build в true)"
    echo "  -g|--gpu                        Оптимизация под NVIDIA Cuda"
    echo "  -h|--help                       Показать справку и выйти"
    echo "__________________________________________________________________________________________________________________________"
    echo "Пример:"
    echo "  $0 -m dev -t rtsp -s"
    echo 
}

# Обрабатываем аргументы
while [ $# -gt 0 ]; do
    case "$1" in
        -m|--mode)
            if [ "$2" = "dev" ]; then
                buildMode=0
            elif [ "$2" = "deb" ]; then
                buildMode=1
            elif [ "$2" = "prod" ]; then
                buildMode=2
            else
                echo "Неизвестный аргумент: $2" >&2
                exit 1
            fi
            shift
        ;;
        -s|--savemake)
            saveMake=1
        ;;
        -g|--gpu)
            gpuOps=1
        ;;
        -h|--help)
            show_help
            exit 0
        ;;
        *)
            echo "Неизвестный ключ: -$1" >&2
            show_help
            exit 1
        ;;
    esac
    shift
done

if [ $saveMake = 0 ]; then
    rm -rf ./*
fi

cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DOS=$OS -DBUILD_MODE=$buildMode -DGPU_OPS=$gpuOps ../..
make