FROM ubuntu:latest as build

USER root

EXPOSE 80

RUN apt-get update && \
    apt-get install -y \
       g++ \
      cmake

COPY . /app

WORKDIR /app/build

RUN cmake .. && cmake --build .

ENTRYPOINT [ "/app/build/httpServer" ]