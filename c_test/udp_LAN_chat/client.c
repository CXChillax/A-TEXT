/************************
*******   client 	*****
************************/

#include "head.h"

int main(int argc, char const *argv[])
{
	pid_t pid;
	int server_fd;
	struct sockaddr_in server_addr;

	if(argc <4)
	{
		fprintf(stderr, "usages: %s ip port name\n", argv[0]);
        exit(-1);
	}

	server_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if( server_fd < 0)
	{
		perror("failed to create server_fd");
		exit(-1);
	}

	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if((pid = fork()) < 0)
    {
    	perror("failed to fork pid ");
    	exit(-1);
    }

    if(pid == 0)
    {
    	recv_message(server_fd);
    }
    else
    {
    	send_message(server_fd, &server_addr, argv[3], pid);
    }

	return 0;
}