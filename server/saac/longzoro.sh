#!/bin/sh
OUT=./longzoro.h
touch main.c
echo "#define SERVER_VERSION \"All Blue's Saserver 8.0 "$(date +%Y��%m��%d��%H:%M:%S)"\n\"" > $OUT
