FROM alpine:3.23.2
LABEL authors="meirelles"

WORKDIR /app

COPY . .

RUN apk add cmake build-base

RUN cmake -S . -B build && cmake --build build