/*********
**server**
*********/

#include "head.h"

int main(int argc, char const *argv[])
{
	int socket_fd;
	pid_t pid;
	struct sockaddr_in server_addr;
	struct node *head;
	int bind_val;
	
	if(argc < 3)
	{
		fprintf(stderr, "usages : %s ip port e.g. %s ipaddr ipaddr \n", argv[0], *argv);
		exit(-1);
	}

	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (socket_fd < 0)
	{
		perror("failed to create socket");
		exit(-1);
	}

	head = create_list();

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	bind_val = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if(bind_val < 0)
	{
		perror("failed to bind");
		exit(-1);
	}

	if((pid = fork()) < 0)
	{
		perror("failed to fork pid");
		exit(-1);
	}

	return 0;
}