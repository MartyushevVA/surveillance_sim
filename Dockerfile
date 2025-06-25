# ======= Этап сборки =======
FROM debian:stable-slim AS builder

# Обновление и установка зависимостей (без лишнего)
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    g++ \
    git \
    libsfml-dev \
    nlohmann-json3-dev \
    ninja-build \
    --fix-missing \
 && rm -rf /var/lib/apt/lists/*


# Создание рабочей директории
WORKDIR /app

# Копирование исходников
COPY . .

# Параметры сборки
ARG ENABLE_TESTS=OFF
ARG TIMING=OFF

# Создание папки сборки и компиляция
RUN mkdir -p build && cd build && \
    cmake .. \
      -DT=${ENABLE_TESTS} \
      -DTIMING=${TIMING} \
      -G Ninja && \
    cmake --build .

# ======= Этап рантайма =======
FROM debian:stable-slim

# Установка только необходимых библиотек для запуска (без компилятора!)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libsfml-dev \
    nlohmann-json3-dev \
 && rm -rf /var/lib/apt/lists/*

# Копирование исполняемого файла из builder-этапа
WORKDIR /app
COPY --from=builder /app/build/sss .
COPY --from=builder /app/view /app/view
COPY --from=builder /app/configs /app/configs
COPY --from=builder /app/Arial.ttf .


# Запуск по умолчанию
CMD ["./sss"]
