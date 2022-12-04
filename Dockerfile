FROM alpine:3.16

RUN apk --no-cache add \
    make \
    python3 \
    git \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    newlib-arm-none-eabi

RUN ln -sf python3 /usr/bin/python

RUN mkdir /home/work

WORKDIR /home/work
