#!/bin/sh
OUT=include/longzoro/longzoro.h

make clean
echo "#ifndef __LONGZORO_H__
#define __LONGZORO_H__

//���⹦�ܿ���----------------------------------------
#define _ATTESTAION_ID 34

#endif" > $OUT
make

tar -zcvf /csa/saserver/gmsv/gmsv_vip_34.tar.gz /csa/saserver/gmsv/gmsvjt /csa/saserver/gmsv/setup.cf 
