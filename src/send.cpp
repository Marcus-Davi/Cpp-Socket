#include <iostream>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 5005


int main(){

		int sock = 0, valread;
		struct sockaddr_in serv_addr;
		char* hello = "Hello from client";

		char buffer[1024] = {0};

		if (( sock = socket(AF_INET , SOCK_STREAM, 0)) < 0){


				printf("Socket creating error \n");
				return -1;
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0 ){
				printf("Invalid IP address \n " );
				return -1;
		}

		if(connect(sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ){
				printf("Could not connect\n");
				return -1;
		}

		
		std::cout << "Connected! Start typing..." << std::endl;
		char send_buffer[100];
		while(1){
		std::cin.getline(send_buffer,sizeof(send_buffer));

		send(sock,send_buffer,sizeof(send_buffer),0);
		valread = read(sock,buffer,1024);
		printf("Server -> %s \n",buffer);
		}

		return 0;
}
