#!/bin/sh
OUT=./longzoro.h
touch callfromac.c
echo "#ifdef _VERSION_NEW" > $OUT
echo "#define SERVER_VERSION \"ʯ��ʱ���ر�� "$(date +%Y��%m��%d��%H:%M:%S)"\"" >> $OUT
echo "#else" >> $OUT
echo "#ifdef _VERSION_25" >> $OUT
echo "#define SERVER_VERSION \"All Blue's Saserver 2.5 "$(date +%Y��%m��%d��%H:%M:%S)"\"" >> $OUT
echo "#else" >> $OUT
echo "#define SERVER_VERSION \"All Blue's Saserver 8.0 "$(date +%Y��%m��%d��%H:%M:%S)"\"" >> $OUT
echo "#endif" >> $OUT
echo "#endif" >> $OUT
