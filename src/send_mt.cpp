#include <iostream>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <poll.h>

#include <thread>

#define PORT 9999

char send_buffer[100];

int sock = 0;

bool receive_thread;

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
			send(sock, send_buffer, sizeof(send_buffer), 0);
		}
	}

	return 0;
}

int main()
{

	int valread;
	struct sockaddr_in serv_addr;

	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{

		printf("Socket creating error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "10.10.10.104", &serv_addr.sin_addr) <= 0)
	{
		printf("Invalid IP address \n ");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Could not connect\n");
		return -1;
	}

	std::cout << "Connected! Start typing..." << std::endl;

	receive_thread = true;
	std::thread t1(rcv_thread);

	int recv_flag = 1;

	while (recv_flag)
	{
		recv_flag = recv(sock, buffer, 1024, 0);
		printf("Other: %s\n", buffer);
		// printf("Flag: %d\n", recv_flag);
	}

	std::cout << "Closing" << std::endl;
	receive_thread = false;
	t1.join();

	return 0;
}
