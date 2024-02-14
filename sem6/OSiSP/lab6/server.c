#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    char username[20];
} Client;

Client clients[MAX_CLIENTS];
pthread_t threads[MAX_CLIENTS];
size_t num_clients = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(client->client_socket, buffer, BUFFER_SIZE, 0)) >
           0) {
        buffer[read_size] = '\0';

        if (buffer[0] == '@') {
            char *recipient_username = strtok(buffer, " ");
            char *message = strtok(NULL, "");
            recipient_username++;
            pthread_mutex_lock(&mutex);
            for (size_t i = 0; i < num_clients; i++) {
                if (strcmp(clients[i].username, recipient_username) == 0) {
                    send(clients[i].client_socket, message, strlen(message), 0);
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_lock(&mutex);
            for (size_t i = 0; i < num_clients; i++) {
                if (clients[i].client_socket != client->client_socket) {
                    send(clients[i].client_socket, buffer, strlen(buffer), 0);
                }
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_mutex_lock(&mutex);
    for (size_t i = 0; i < num_clients; i++) {
        if (clients[i].client_socket == client->client_socket) {
            clients[i] = clients[num_clients - 1];
            break;
        }
    }
    num_clients--;
    pthread_mutex_unlock(&mutex);

    close(client->client_socket);
    free(client);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {
        perror("Ошибка связывания сокета с адресом");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Ошибка прослушивания сокета");
        exit(1);
    }

    printf("Сервер запущен. Ожидание подключений...\n");

    while (1) {
        addr_size = sizeof(client_addr);

        client_socket =
            accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Ошибка принятия соединения");
            continue;
        }

        char username[20];
        if (recv(client_socket, username, sizeof(username), 0) <= 0) {
            perror("Ошибка получения имени пользователя");
            close(client_socket);
            continue;
        }

        Client *client = (Client *)malloc(sizeof(Client));
        client->client_socket = client_socket;
        strncpy(client->username, username, sizeof(client->username));

        pthread_mutex_lock(&mutex);
        if (num_clients < MAX_CLIENTS) {
            int is_name_unique = 1;

            for (size_t i = 0; i < num_clients; i++) {
                if (strcmp(clients[i].username, client->username) == 0) {
                    char reject_msg[100] = "Отказано в подключении. Имя не уникально\n";
                    send(client->client_socket, reject_msg, strlen(reject_msg),
                         0);
                    close(client_socket);
                    is_name_unique = 0;
                    break;
                }
            }
            if (is_name_unique) {
                clients[num_clients] = *client;
                num_clients++;
                printf("Клиент %s подключен\n", client->username);
            }
        } else {
            printf(
                "Превышено максимальное количество клиентов. Клиент %s "
                "отклонен\n",
                client->username);
            char reject_msg[100] = "Отказано в подключении. Слишком много клиентов\n";
            send(client->client_socket, reject_msg, strlen(reject_msg), 0);
            free(client);
            close(client_socket);
            continue;
        }
        pthread_mutex_unlock(&mutex);

        if (pthread_create(&threads[num_clients - 1], NULL, handle_client,
                           (void *)client) != 0) {
            perror("Ошибка создания потока");
            free(client);
            close(client_socket);
            continue;
        }
    }

    close(server_socket);

    return 0;
}