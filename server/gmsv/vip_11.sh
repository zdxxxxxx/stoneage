#!/bin/sh
OUT=include/longzoro/longzoro.h

make clean
echo "#ifndef __LONGZORO_H__
#define __LONGZORO_H__

//���⹦�ܿ���----------------------------------------
#define _ATTESTAION_ID 11

#endif" > $OUT
make

tar -zcvf /csa/saserver/gmsv/gmsv_vip_11.tar.gz /csa/saserver/gmsv/gmsvjt /csa/saserver/gmsv/setup.cf 
