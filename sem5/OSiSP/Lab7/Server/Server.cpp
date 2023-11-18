#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 10
#define BUFFER_SIZE 4096

SOCKET clients[MAX_CLIENTS];
int numClients = 0;

void AcceptConnections(SOCKET serverSocket);
void ProcessClientMessage(SOCKET clientSocket);
void BroadcastMessage(SOCKET clientSocket, char* message, int messageLength);

int main() {
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WinSock initialization error." << std::endl;
		return -1;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket == INVALID_SOCKET) {
		std::cout << "Socket creation error." << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(8888); 

	if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cout << "Socket binding error." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}

	if(listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "Socket listening error." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	std::cout << "Server is running..." << std::endl;

	AcceptConnections(serverSocket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}

void AcceptConnections(SOCKET serverSocket) {
	while(true) {
		SOCKET clientSocket = accept(serverSocket, NULL, NULL);
		if(clientSocket == INVALID_SOCKET) {
			std::cout << "Accept connection error." << std::endl;
			continue;
		}

		if(numClients < MAX_CLIENTS) {
			clients[numClients] = clientSocket;
			numClients++;

			std::cout << "Client connected. Total clients: " << numClients << std::endl;

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ProcessClientMessage, (LPVOID)clientSocket, NULL, NULL);
		}
		else {
			std::cout << "Server has reached its client limit. Connection refused." << std::endl;
			closesocket(clientSocket);
		}
	}
}

void ProcessClientMessage(SOCKET clientSocket) {
	while(true) {
		char* message = new char[BUFFER_SIZE];
		int bytesRead = recv(clientSocket, message, BUFFER_SIZE-1, 0);
		if(bytesRead <= 0) {
			for(int i = 0; i < numClients; i++) {
				if(clients[i] == clientSocket) {
					closesocket(clientSocket);
					std::cout << "Client disconnected. Total clients: " << numClients - 1 << std::endl;

					for(int j = i; j < numClients - 1; j++) {
						clients[j] = clients[j + 1];
					}

					numClients--;
					break;
				}
			}

			break;
		}

		if(bytesRead > 0)
			BroadcastMessage(clientSocket, message, bytesRead);
		delete[] message;
	}
}

void BroadcastMessage(SOCKET clientSocket, char* message, int messageLength) {
	int idx = 0;
	for(int i = 0; i < numClients; i++) {
		if(clients[i] == clientSocket) {
			idx = i;
			break;
		}
	}
	std::string modifiedMessage = "client #" + std::to_string(idx) + ": " + std::string(message);
	int lenIdx = std::to_string(idx).size();
	int len = 10 + lenIdx + messageLength;
	for(int i = 0; i < numClients; i++) {
		if(clients[i] == clientSocket) continue;
		if(send(clients[i], modifiedMessage.c_str(), len, 0) == SOCKET_ERROR) {
			std::cout << "Error sending message to client." << std::endl;
		}
	}
}