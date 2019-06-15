#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#define PORT 9999

int main(void)
{
	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_info;

	int init_msg = 1;

	char client_msg[255];
	char server_res[255];

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(PORT);
	server_info.sin_addr.s_addr = INADDR_ANY;

	if (connect(client_sock, (struct sockaddr *)&server_info, sizeof(server_info)))
	{
		printf("[-] Error: Socket connection\n");
		return -1;
	}

	printf("[+] Connected with server! Type \"Hello\" to start conversation.\n");
	printf("[+] Type \"Bye\" to end conversation.\n");

	printf("CLIENT: ");
	fgets(client_msg, sizeof(client_msg), stdin); fflush(stdin);
	client_msg[strlen(client_msg) - 1] = '\0';
	send(client_sock, client_msg, sizeof(client_msg), 0); usleep(1000);

	while (1)
	{
		// Detect whether socket is closed by server (wrong initiation or "Bye")
		if (!recv(client_sock, &server_res, sizeof(server_res), 0))
		{
			// If it is the initial message that caused server socket closure,
			// close connection and exit program
			if (init_msg)
			{
				printf("[-] Wrong initiation: \"Hello\" expected\n");
				close(client_sock);
				return -1;
			}
			// If it is the "Bye" message that caused server socket closure
			break;
		}
		printf("SERVER: %s\n", server_res);

		printf("CLIENT: "); fgets(client_msg, sizeof(client_msg), stdin); fflush(stdin);
		client_msg[strlen(client_msg) - 1] = '\0';
		send(client_sock, client_msg, sizeof(client_msg), 0); usleep(1000);
		init_msg = 0;
	}

	close(client_sock);
	printf("[*] Closed socket, exiting program...\n");

	return 0;
}

