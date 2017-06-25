#ifndef _HEAD_H_
#define _HEAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define CLIENT_LOGIN    100
#define CLIENT_CHAT     200
#define CLIENT_QUIT     300

#define SERVER_CHAT     400
#define SERVER_QUIT     500

#define NAME_SIZE		20
#define MESSAGE_TEXT	512

#endif

//定义客户端节点
struct node
{
	char name[NAME_SIZE];
	struct sockaddr_in client_addr;
	struct node *next;
};

//消息结构体成员组成
struct message
{
	long type;
	char name[NAME_SIZE];
	char mtext[MESSAGE_TEXT];
};

/**********************    server   ****************/

//初始化客户端节点链表
struct node *create_list(void);
//添加一个客户端
void insert_list(struct node *, char *, struct sockaddr_in *);
//删除一个客户端
void delte_list(struct node *, char *);

//接收消息
void server_recv_message(int , struct node *);
//发送消息
void server_send_message(int , struct sockaddr_in *, pid_t );

//客户端链接
void client_login(int, struct node *, struct message *, struct sockaddr_in);
void client_chat(int , struct node *, struct message *);
void client_quit(int , struct node *, struct message *);
//服务器
void server_chat(int , struct node *, struct message *);
void server_quit(int , struct node *, struct message *);

//brocast 直播？？
void brocast_msg(int , struct node *, struct message *);

void father_func(int sig_no)
{
	return ;
}

/***************   client  ******************/
void client_recv_message(int );
void client_send_message(int , struct sockaddr_in *, pit_t );

void login_msg(struct message *);
void group_msg(struct message *);
void quit_msg(struct message *);
void server_msg(struct message *);
void client_server_quit(void);

