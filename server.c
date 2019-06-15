#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define PORT 9999

// socket -> bind -> listen -> accept

int main(void)
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	int conn_sock;
	struct sockaddr_in server_info;

	char server_msg[255] = "Successful initiation!";
	char client_res[255];

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(PORT);
	server_info.sin_addr.s_addr = INADDR_ANY;

	bind(server_sock, (struct sockaddr*)&server_info, sizeof(server_info));

	listen(server_sock, 7);

	conn_sock = accept(server_sock, NULL, NULL);
	printf("[+] Connected with client!\n");

	recv(conn_sock, &client_res, sizeof(client_res), 0);
	printf("CLIENT: %s\n", client_res);

	// Check if initial message from client is "Hello"
	if (strncmp(client_res, "Hello", sizeof(client_res)))
	{
		printf("[-] Wrong initiation: \"Hello\" expected\n");
		close(conn_sock);
		close(server_sock);
		return -1;
	}

	// Send initial welcome message defined at the top of program
	send(conn_sock, server_msg, sizeof(server_msg), 0); usleep(1000);
	printf("SERVER: %s\n", server_msg);

	while (1)
	{
		recv(conn_sock, &client_res, sizeof(client_res), 0);
		printf("CLIENT: %s\n", client_res);
		if (!strncmp(client_res, "Bye", sizeof(client_res))) break;

		printf("SERVER: "); fgets(server_msg, sizeof(server_msg), stdin); fflush(stdin);
		server_msg[strlen(server_msg) - 1] = '\0';
		send(conn_sock, server_msg, sizeof(server_msg), 0); usleep(1000);
	}

	close(conn_sock);
	close(server_sock);

	printf("[*] Closed socket, exiting program...\n");

	return 0;
}

