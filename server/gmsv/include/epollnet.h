/*
 * epollnet.h
 *
 *  Created on: 2015��10��23��
 *      Author: hult
 */

#ifndef EPOLLNET_H_
#define EPOLLNET_H_
#ifdef _EPOLL_ET_MODE
//��epoll�����˿�
int epoll_bind(int port);
//��acfd��ӵ�epoll��
int epoll_add_acfd(int acfd);
//������
int epoll_set_nonblock(int fd) ;
//ע�ᵽepoll
int epoll_new_conn(int sfd) ;
//��epoll���Ƴ�
int epoll_close_conn(int sfd);
//��epoll
int epoll_mod_read(int sfd) ;
//дepoll
int epoll_mod_write(int sfd) ;

//������������߳�
int Start_PacketWrapper();
//����epoll��ѭ���߳�
int Start_Epoll_Loop();
//int Start_Epoll_SendLoop();
#endif
#endif /* EPOLLNET_H_ */
