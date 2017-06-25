
/*
sockaddr_in（在netinet/in.h中定义）：
struct sockaddr_in {
	short int sin_family;                       Address family 
	unsigned short int sin_port;        Port number 
	struct in_addr sin_addr;              Internet address 
	unsigned char sin_zero[8];        Same size as struct sockaddr 
};
struct in_addr {
	unsigned long s_addr;	
};
typedef struct in_addr {
union {
        struct{
            unsigned char s_b1,
            s_b2,
            s_b3,
            s_b4;
            } S_un_b;

       struct {
                unsigned short s_w1,
                s_w2;
                } S_un_w;

        unsigned long S_addr;
      } S_un;
} IN_ADDR;
sin_family指代协议族，在socket编程中只能是AF_INET
sin_port存储端口号（使用网络字节顺序）
sin_addr存储IP地址，使用in_addr这个数据结构
sin_zero是为了让sockaddr与sockaddr_in两个数据结构保持大小相同而保留的空字节。
s_addr按照网络字节顺序存储IP地址
sockaddr_in和sockaddr是并列的结构，指向sockaddr_in的结构体的指针也可以指向
sockadd的结构体，并代替它。也就是说，你可以使用sockaddr_in建立你所需要的信息,
在最后用进行类型转换就可以了bzero((char*)&mysock,sizeof(mysock));//初始化
mysock结构体名
mysock.sa_family=AF_INET;
mysock.sin_addr.s_addr=inet_addr("192.168.0.1");
……
等到要做转换的时候用：
（struct sockaddr*）mysock

*/

#include "head.h"


/***************************  server   **********************************/

//初始化客户端节点链表
struct node *create_list(void)
{
	struct node *head = (struct node *)malloc(sizeof(struct node));
	head->next = NULL;

	return head;
};

//添加一个客户端
void insert_list(struct node *head, char *name, struct sockaddr_in *client_addr)
{
	struct node *new = (struct node *)malloc(sizeof(struct node));

	strcpy(new->name, name);
	new->client_addr = *client_addr;

	new->next = head->next;
	head->next = new;
}

//删除一个客户端
void delte_list(struct node *head, char *name)
{
	struct node *p = head->next;
	struct node *q = head;

	while(p != NULL)
	{
		if(strcmp(p->name, name) == 0)
			break;

		p = p->next;
		q = q->next;
	}

	q->next = p->next;
	p->next = NULL;
	free(p);
}

//接收消息
void server_recv_message(int socket_fd, struct node *head)
{
	struct message msg;
	struct sockaddr_in client_addr;
	int client_addrlen = sizeof(struct sockaddr);

	int recv_val;

	while(1)
	{
		recv_val = recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&client_addr, &client_addrlen);
		if(recv_val < 0)
		{
			perror("failed to recvfrom client");
			exit(-1);
		}

		switch(msg.type)
		{
			case CLIENT_LOGIN:
				client_login(socket_fd, head, &msg, &client_addr);
				break;

			case CLIENT_CHAT:
				client_chat(socket_fd, head, &msg);
				break;

			case CLIENT_QUIT:
				client_quit(socket_fd, head, &msg);
				break;

			case SERVER_CHAT:
				server_chat(socket_fd, head, &msg);
				break;

			case SERVER_QUIT:
				server_quit(socket_fd, head, &msg);
				break;

			default:
				break;
		}
	}
}

//发送消息
void server_send_message(int socket_fd, struct sockaddr_in *server_addr, pid_t pid)
{
	struct message msg;
	char buf[MESSAGE_TEXT];
	int sendto_val;
	signal(getppid(), father_func);

	while(1)
	{
		usleep(500);
		printf(">");
		fgets(buf, sizeof(buf), stdin);

		buf[strlen(buf)-1] = 0;			//处理:有效信息的后一个元素为‘0’

		strcpy(msg.mtext, buf);
		strcpy(msg.name, "server");
		msg.type = SERVER_CHAT;

		if(strncmp(buf, "quit", 4) == 0 || strncmp(buf, "exit", 4) == 0)		//strncmp函数判断退出字符串
		{
			msg.type = SERVER_QUIT;
			sendto_val = sendto(socket_fd, &msg, sizeof(msg), 0, (struct socket_fd *)sockaddr_addr, sizeof(struct sockaddr));

			if (sendto_val < 0)
			{
				perror("failedn to send server_quit message");
				exit(-1);
			}

			pause();

			kill(pid, SIGKILL);
			waitpid(pid , NULL, 0);
			exit(0);
		}

		sendto_val = sendto(socket_fd, &msg, sizeof(msg), 0, (struct socket_fd *)sockaddr_addr, sizeof(struct sockaddr));
		
		if(sendto_val < 0)
		{
			perror("failed to send server_char message");
			exit(-1);
		}

	}
}

//客户端链接
void client_login(int socket_fd, struct node *head, struct message *msg, struct sockaddr_in *client_addr)
{
	printf("********Login In********\n");
    printf("%s is Login In\n", msg->name);
    printf("************************\n");

    insert_list(head, msg->name, client_addr);
    brocast_msg(socket_fd, head, msg);
}
void client_chat(int socket_fd, struct node *head, struct message *msg)
{
	printf("********Group Chat********\n");
    printf("name: %s\n", msg->name);
    printf("msg: %s\n", msg->mtext);
    printf("**************************\n");

    brocast_msg(socket_fd, head, msg);
}
void client_quit(int socket_fd, struct node *head, struct message *msg)
{
	printf("*********Quit Msg********\n");
    printf("%s is Quit\n", msg->name);
    printf("*************************\n");

    delete_list(head, msg->name);
    brocast_msg(socket_fd, head, msg);
}

//服务器
void server_chat(int socket_fd, struct node *head, struct message *msg)
{
	printf("********Server Msg*********\n");
    printf("msg: %s\n", msg->mtext);
    printf("***************************\n");

    brocast_msg(socket_fd, head, msg);
}
void server_quit(int socket_fd, struct node *head, struct message *msg)
{
	brocast_msg(socket_fd, head, msg);
    kill(getppid(), SIGUSR1);
}

//brocast 直播？？
void brocast_msg(int socket_fd, struct node *head, struct message *msg)
{
	struct node *p = head_>next;

	while(p != NULL)
	{
		if(msg->type == CLIENT_LOGIN)
		{
			if (strcmp(p->name, msg->name) == 0)
			{
				p = p->next;
				continue;
			}
		}

		sendto(socket_fd, msg, sizeof(struct message), 0, (struct socketaddr *)&(p->client_addr), sizeof(struct sockaddr));

		p= p->next;
	}
}


/****************************   client  ***********************************/

void recv_message(int server_fd)
{
	struct message msg;
	int recv_val;

	while(1)
	{
		memset(&msg, 0, sizeof(msg));

		recv_val = recvfrom(server_fd, &msg, sizeof(msg), 0, NULL, NULL);
		if (recv_val < 0)
		{
			perror("failed to recv server message");
			exit(-1);
		}

		switch(msg.type)
		{
			case CLIENT_LOGIN:
				login_msg(&msg);
				break;

			case CLIENT_CHAT:
				group_msg(&msg);
				break;

			case CLIENT_QUIT:
				quit_msg(&msg);
				break;

			case SERVER_CHAT:
				server_msg(&msg);
				break;

			case SERVER_QUIT:
				server_quit();
				break;

			default:
				break;	
		}
	}
}

//
void send_message(int server_fd, struct sockaddr_in *server_addr, char *name, pid_t pid)
{
	struct message msg;
	char buf[MESSAGE_TEXT];
	int sendto_val;

	msg.type = CLIENT_LOGIN;
	strcpy(msg.name, name);

	sendto(server_fd, &msg, sizeof(msg), 0, (struct sockaddr *)server_addr, sizeof(struct sockaddr));
	if(sendto_val < 0)
	{
		perror("failed to send login message");
		exit(-1);
	}

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		memset(&msg, 0, sizeof(msg));			//值得注意和商榷的问题

		usleep(500);
		printf(">");
		fgets(buf, sizeof(buf), stdin);
		buf[sizeof(buf)-1] = 0;

		strcpy(msg.mtext, buf);
		strcpy(msg.name, name);
		msg.type = CLIENT_CHAT;

		if(strncmp(buf, "quit", 4) == 0 || strncmp(buf, "exit", 4) == 0)
		{
			msg.type = CLIENT_QUIT;
			
			sendto_val = sendto(server_fd, &msg, sizeof(msg), 0, (struct sockaddr *)server_addr, sizeof(struct sockaddr));

			if(sendto_val < 0)
			{
				perror("failed to send quit message");
				exit(-1);
			}

			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			exit(0);
		}

		sendto_val = sendto(server_fd, &msg, sizeof(msg), 0, (struct sockaddr *)server_addr, sizeof(struct sockaddr));

		if(sendto_val < 0)
		{
			perror("failed to send group message");
			exit(-1);
		}
	}
}

//
void login_msg(struct message *msg)
{
    printf("######## Login in ########\n");
    printf("%s is login in\n", msg->name);
    printf("######## Login in ########\n");

}

void group_msg(struct message *msg)
{
    printf("******** Group Msg ********\n");
    printf("name: %s\n", msg->name);
    printf("msg: %s\n", msg->mtext);
    printf("******** Group Msg ********\n");

}

void quit_msg(struct message *msg)
{
    printf("######## Quit Msg ########\n");
    printf("%s is Quit\n", msg->name);
    printf("######## Quit Msg ########\n");

}

void server_msg(struct message *msg)
{
    printf("******** Server Msg ********\n");
    printf("msg: %s\n", msg->mtext);
    printf("******** Server Msg ********\n");

}

void server_quit(void )
{
    kill(getppid(), SIGKILL);
    exit(0);
}