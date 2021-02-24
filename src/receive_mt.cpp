
// Server side C/C++ program to demonstrate Socket programming
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <thread>
#include <string.h>
#include <poll.h>

#define PORT 5005

bool receive_thread = false;

int server_fd, new_socket;

int rcv_thread()
{

	struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};

	int ret;
	static char send_buffer[100];

	while (receive_thread)
	{
		// std::cin.getline(send_buffer, sizeof(send_buffer));
		ret = poll(&pfd, 1, 50);

		if (ret == 1)
		{
			std::cin.getline(send_buffer, sizeof(send_buffer));
			send(new_socket, send_buffer, sizeof(send_buffer), 0);
		}
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	//int valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				   &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	std::cout << "Waiting..." << std::endl;
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
							 (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	receive_thread = true;
	std::thread t1(rcv_thread);

	std::cout << "Connected!" << std::endl;
	std::string output_msg = "ok!";
	int recv_flag = 1;

	// printf("NS : %d", new_socket);
	// printf("SFD : %d", server_fd);

	// TODO detect connection close
	while (recv_flag)
	{
		recv_flag = recv(new_socket, buffer, 1024, 0);
		printf("Other: %s\n", buffer);
		// printf("Flag: %d\n", recv_flag);
	}
	std::cout << "Closing" << std::endl;
	receive_thread = false;
	t1.join();

	return 0;
}
