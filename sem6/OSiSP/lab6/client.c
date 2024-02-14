#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void *receive_messages(void *arg) {
    int server_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(server_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("%s\n", buffer);
    }

    pthread_exit(NULL);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    // Создаем сокет
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Настраиваем адрес сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Устанавливаем соединение с сервером
    if (connect(server_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        perror("Ошибка соединения с сервером");
        exit(1);
    }

    // Получаем имя пользователя от пользователя
    char username[20];
    printf("Введите ваше имя пользователя: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    // Отправляем имя пользователя серверу
    if (send(server_socket, username, sizeof(username), 0) == -1) {
        perror("Ошибка отправки имени пользователя");
        close(server_socket);
        exit(1);
    }

    pthread_t thread;
    if (pthread_create(&thread, NULL, receive_messages,
                       (void *)&server_socket) != 0) {
        perror("Ошибка создания потока");
        close(server_socket);
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (send(server_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Ошибка отправки сообщения");
            break;
        }
    }
    close(server_socket);

    return 0;
}