# Базовый образ с CMake, GCC и необходимыми библиотеками
FROM ubuntu:latest

# Установим зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libsfml-dev \
    libjsoncpp-dev \
    libgtest-dev \
    nlohmann-json3-dev \
    ninja-build \
    x11-apps \
    && apt-get clean

# GTest требует компиляции, если не используем CMake FetchContent
RUN cd /usr/src/gtest && cmake . && make && cp lib/*.a /usr/lib

# Рабочая директория
WORKDIR /app

# Скопировать все в контейнер
COPY . .

# Создаем build-папку
RUN mkdir -p build
WORKDIR /app/build

# Можно передавать флаги через ARG
ARG ENABLE_TESTS=OFF
ARG TIMING=OFF

# Конфигурация проекта (включаем или отключаем тесты)
RUN cmake .. \
    #-DCMAKE_BUILD_TYPE=Release \
    -DT=${ENABLE_TESTS} \
    -DTIMIG=${TIMING} \
    -G Ninja \
 && cmake --build .

# По умолчанию запускаем приложение (можно переопределить в docker run)
CMD ["./sss"]

#sudo docker build -t surv_sys .
#xhost +local:docker
#sudo docker run --rm -e DISPLAY=$DISPLAY -e XDG_RUNTIME_DIR=/tmp/runtime -v /tmp/.X11-unix:/tmp/.X11-unix surv_sys
#xhost -local:docker