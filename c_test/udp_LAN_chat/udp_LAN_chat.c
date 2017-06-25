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


struct node 
{
    char name[20];
    struct sockaddr_in client_addr;
    struct node *next;
};

struct message
{
    long type;
    char name[20];
    char mtext[512];
};

struct node *create_list(void);
void insert_list(struct node *, char *, struct sockaddr_in *);
void delete_list(struct node *, char *);

void recv_message(int , struct node *);
void send_message(int , struct sockaddr_in *, pid_t );

void client_login(int , struct node *, struct message *, struct sockaddr_in *);
void client_chat(int , struct node *, struct message *);
void client_quit(int , struct node *, struct message *);
void server_chat(int , struct node *, struct message *);
void server_quit(int , struct node *, struct message *);

void brocast_msg(int , struct node *, struct message *);

void father_func(int sig_no)
{
    return ;
}

int main(int argc, const char *argv[])
{
    int socket_fd;
    pid_t pid;
    struct sockaddr_in server_addr;
    struct node *head;

    if (argc < 3)
    {
        fprintf(stderr, "usages : %s ip port\n", argv[0]);
        exit(-1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("failed to create socket");
        exit(-1);
    }

    head = create_list();

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("failed to bind");
        exit(-1);
    }

    if ((pid = fork()) < 0)              //创建子经常
    {
        perror("failed to fork pid");
        exit(-1);
    }

    if (pid == 0)
        recv_message(socket_fd, head);
    else
        send_message(socket_fd, &server_addr, pid);

    return 0;
}


struct node *create_list(void)
{
    struct node *head;

    head = (struct node *)malloc(sizeof(struct node));
    head->next = NULL;

    return head;
}

void insert_list(struct node *head, char *name, struct sockaddr_in *client_addr)
{
    struct node *new;

    new = (struct node *)malloc(sizeof(struct node));
    strcpy(new->name, name);
    new->client_addr = *client_addr;

    new->next = head->next;
    head->next = new;

    return ;
}

void delete_list(struct node *head, char *name)
{
    struct node *p = head->next;
    struct node *q = head;

    while (p != NULL)
    {
        if (strcmp(p->name, name) == 0)
            break;

        p = p->next;
        q = q->next;
    }

    q->next = p->next;
    p->next = NULL;
    free(p);

    return ;
}

void recv_message(int socket_fd, struct node *head)
{
    struct message msg;
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(struct sockaddr);

    while (1)
    {
        if (recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&client_addr, &client_addrlen) < 0)
        {
            perror("failed to recvform client");
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

    return ;
}

void send_message(int socket_fd, struct sockaddr_in *server_addr, pid_t pid)
{
    struct message msg;
    char buf[512];

    signal(getppid(), father_func);
    
    while (1)
    {
        usleep(500);
        printf(">");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = 0;

        strcpy(msg.mtext, buf);
        strcpy(msg.name , "server");
        msg.type = SERVER_CHAT;

        if (strncmp(buf, "quit", 4) == 0)
        {
            msg.type = SERVER_QUIT;

            if (sendto(socket_fd, &msg, sizeof(msg), 0, 
                        (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
            {
                perror("failed to send server_quit message");
                exit(-1);
            }

            pause();

            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            exit(0);
        }

        if (sendto(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
        {
            perror("failed to send server_chat message");
            exit(-1);
        }
    }

    return ;
}

void client_login(int socket_fd, struct node *head, struct message *msg, struct sockaddr_in *client_addr)
{
    printf("********Login In********\n");
    printf("%s is Login In\n", msg->name);
    printf("************************\n");

    insert_list(head, msg->name, client_addr);
    brocast_msg(socket_fd, head, msg);

    return ;
}

void client_chat(int socket_fd, struct node *head, struct message *msg)
{
    printf("********Group Chat********\n");
    printf("name: %s\n", msg->name);
    printf("msg: %s\n", msg->mtext);
    printf("**************************\n");

    brocast_msg(socket_fd, head, msg);

    return ;
}

void client_quit(int socket_fd, struct node *head, struct message *msg)
{
    printf("*********Quit Msg********\n");
    printf("%s is Quit\n", msg->name);
    printf("*************************\n");

    delete_list(head, msg->name);
    brocast_msg(socket_fd, head, msg);

    return ;
}

void server_chat(int socket_fd, struct node *head, struct message *msg)
{
    printf("********Server Msg*********\n");
    printf("msg: %s\n", msg->mtext);
    printf("***************************\n");

    brocast_msg(socket_fd, head, msg);

    return ;
}

void server_quit(int socket_fd, struct node *head, struct message *msg)
{
    brocast_msg(socket_fd, head, msg);
    kill(getppid(), SIGUSR1);

    return ;
}

void brocast_msg(int socket_fd, struct node *head, struct message *msg)
{
    struct node *p = head->next;

    while(p != NULL)
    {
        if (msg->type == CLIENT_LOGIN)
        {
            if (strcmp(p->name, msg->name) == 0)
            {
                p = p->next;
                continue;
            }
        }

        sendto(socket_fd, msg, sizeof(struct message), 0, (struct sockaddr *)&(p->client_addr), sizeof(struct sockaddr));
        p = p->next;
    }

    return ;
}














































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

struct message
{
    long type;
    char name[20];
    char mtext[512];
};

void recv_message(int );
void send_message(int , struct sockaddr_in *, char *, pid_t);

void login_msg(struct message *);
void group_msg(struct message *);
void quit_msg(struct message *);
void server_msg(struct message *);
void server_quit(void);

int main(int argc, char *argv[])
{
    pid_t pid;
    int server_fd;
    struct sockaddr_in server_addr;

    if (argc < 4)
    {
        fprintf(stderr, "usages: %s ip port name\n", argv[0]);
        exit(-1);
    }

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("failed to create server_fd");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if ((pid = fork()) < 0)
    {
        perror("failed to fork pid");
        exit(-1);
    }

    if (pid == 0)
        recv_message(server_fd);
    else
        send_message(server_fd, &server_addr, argv[3], pid);

    return 0;
}

void recv_message(int server_fd)
{
    struct message msg;	

    while (1)
    {
        memset(&msg, 0, sizeof(msg));

        if (recvfrom(server_fd, &msg, sizeof(msg), 0, NULL, NULL) < 0)
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

    return ;
}

void send_message(int server_fd, struct sockaddr_in *server_addr, char *name, pid_t pid)
{
    struct message msg;
    char buf[512];

    msg.type = CLIENT_LOGIN;
    strcpy(msg.name, name);

    if (sendto(server_fd, &msg, sizeof(msg), 0, 
                (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("failed to send login message");
        exit(-1);
    }

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        memset(&msg, 0, sizeof(msg));

        usleep(500);
        printf(">");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = 0;

        strcpy(msg.mtext, buf);
        strcpy(msg.name, name);
        msg.type = CLIENT_CHAT;

        if (strncmp(buf, "quit", 4) == 0)
        {
            msg.type = CLIENT_QUIT;

            if (sendto(server_fd, &msg, sizeof(msg), 0, 
                        (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
            {
                perror("failed to send quit message");
                exit(-1);
            }

            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            exit(0);
        }

        if (sendto(server_fd, &msg, sizeof(msg), 0, 
                    (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
        {
            perror("failed to send group message");
            exit(-1);
        }
        
    }

    return ;
}

void login_msg(struct message *msg)
{
    printf("######## Login in ########\n");
    printf("%s is login in\n", msg->name);
    printf("######## Login in ########\n");

    return ;
}

void group_msg(struct message *msg)
{
    printf("******** Group Msg ********\n");
    printf("name: %s\n", msg->name);
    printf("msg: %s\n", msg->mtext);
    printf("******** Group Msg ********\n");

    return ;
}

void quit_msg(struct message *msg)
{
    printf("######## Quit Msg ########\n");
    printf("%s is Quit\n", msg->name);
    printf("######## Quit Msg ########\n");

    return ;
}

void server_msg(struct message *msg)
{
    printf("******** Server Msg ********\n");
    printf("msg: %s\n", msg->mtext);
    printf("******** Server Msg ********\n");

    return ;
}

void server_quit(void )
{
    kill(getppid(), SIGKILL);
    exit(0);
}
