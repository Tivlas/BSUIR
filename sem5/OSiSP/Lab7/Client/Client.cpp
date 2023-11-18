#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

void ReceiveMessages(SOCKET serverSocket);
void SendMessageToServer(SOCKET serverSocket, const char* message);

int main() {
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WinSock initialization error." << std::endl;
		return -1;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket == INVALID_SOCKET) {
		std::cout << "Socket creation error." << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(8888);

	if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cout << "Connection to server error." << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	std::cout << "Successfully connected to server." << std::endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReceiveMessages, (LPVOID)clientSocket, NULL, NULL);

	char message[BUFFER_SIZE];
	while(true) {
		std::cin.getline(message, BUFFER_SIZE - 1);
		SendMessageToServer(clientSocket, message);
	}
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}

void ReceiveMessages(SOCKET serverSocket) {
	char message[BUFFER_SIZE];
	while(true) {
		int bytesRead = recv(serverSocket, message, BUFFER_SIZE, 0);
		if(bytesRead <= 0) {
			std::cout << "Disconnected from server." << std::endl;
			break;
		}
		if(bytesRead > 0) {
			message[bytesRead] = '\0';
			std::cout << message << std::endl;
		}
	}
}

void SendMessageToServer(SOCKET serverSocket, const char* message) {
	if(send(serverSocket, message, strlen(message), 0) == SOCKET_ERROR) {
		std::cout << "Error sending message to server." << std::endl;
	}
}