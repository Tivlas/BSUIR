#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <iostream>

#define LISTEN_BACKLOG 50
#define handle_error_en(en, msg) \
    do {                         \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)

bool is_connected = false;
int sock_client;
std::atomic<int> G_CNT(0);

struct pseudo_header  // for checksum
{
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;

    struct tcphdr tcp;
};

unsigned short csum(unsigned short *ptr, int nbytes) {
    long sum;
    unsigned short oddbyte;
    short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;

    return (answer);
}

void *tcp_shk(void *arg) {
    char datagram[4096], source_ip[32];
    // IP header
    struct iphdr *iph = (struct iphdr *)datagram;
    // TCP header
    struct tcphdr *tcph = (struct tcphdr *)(datagram + sizeof(struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;

    strcpy(source_ip, "127.0.0.1");

    sin.sin_family = AF_INET;
    sin.sin_port = htons(8000);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(datagram, 0, 4096);

    // IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
    iph->id = htons(54321);  // Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;
    iph->saddr = inet_addr(source_ip);
    iph->daddr = sin.sin_addr.s_addr;

    iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);

    // TCP Header
    tcph->source = htons(1234);
    tcph->dest = htons(8000);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;  // tcp segment
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 1;
    tcph->urg = 0;
    tcph->window = htons(5840);
    tcph->check = 0;
    tcph->urg_ptr = 0;
    // Now the IP checksum

    psh.source_address = inet_addr(source_ip);
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);

    memcpy(&psh.tcp, tcph, sizeof(struct tcphdr));

    tcph->check = csum((unsigned short *)&psh, sizeof(struct pseudo_header));

    send(sock_client, datagram, strlen(datagram), 0);

    printf("send SYN+ACK, waiting for ACK ");
    char msg[4000];
    G_CNT++;
    std::cout << G_CNT << std::endl;

    ssize_t err = recv(-1, msg, 4000, 0);
    pthread_exit(0);
}

void *server_receive(void *arg) {
    char msg[4000];

    while (1) {
        ssize_t err = recv(sock_client, msg, 4000, 0);

        if (err > 0) {
            msg[4000] = '\0';
            pthread_t thread;
            pthread_create(&thread, NULL, tcp_shk, NULL);
            if (err == -1) handle_error_en(-1, "send");
        }

        if (err = 0) {
            break;
        }

        if (err == -1) {
            handle_error_en(err, "recv");
        }
    }

    pthread_exit(0);
}

int main(int argc, char **argv) {
    system("clear");
    pthread_attr_t attr;
    int err = pthread_attr_init(&attr);

    if (err != 0) handle_error_en(err, "pthread_attr_init");

    int socket_descr = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descr == -1) handle_error_en(socket_descr, "socket(2)");

    struct sockaddr_in server_sa;
    server_sa.sin_family = AF_INET;
    server_sa.sin_port = htons(8000);
    server_sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket_descr, (struct sockaddr *)&server_sa, sizeof(server_sa)) ==
        -1) {
        handle_error_en(-1, "bind");
        return 1;
    }

    if (listen(socket_descr, LISTEN_BACKLOG) == -1) {
        handle_error_en(-1, "listen");
        return 1;
    }

    int *client = (int *)malloc(sizeof(int));
    int client_count = 0;
    struct client_info **cinfo_arr =
        (struct client_info **)malloc(sizeof(struct client_info *) * 100);
    int client_info_count = 0;

    char *server_addr = inet_ntoa(server_sa.sin_addr);
    in_port_t server_port = server_sa.sin_port;
    printf("Server is ready.\n ip: %s:%d\n", server_addr, server_port);

    while (1) {
        struct sockaddr_in client_sa;
        socklen_t peer_addr_size = sizeof(struct sockaddr_in);
        int sck = accept(socket_descr, (struct sockaddr *)&client_sa,
                         (socklen_t *)&peer_addr_size);
        char *client_addr = inet_ntoa(client_sa.sin_addr);
        in_port_t client_port = client_sa.sin_port;

        if (!is_connected && sck != -1) {
            sock_client = sck;
            printf("New connection: %s:%d\n", client_addr, client_port);
            pthread_t thread;
            pthread_create(&thread, &attr, server_receive, NULL);

            is_connected = 1;
            printf("%d\n", client_count++);
            fflush(stdout);
        }
    }

    err = pthread_attr_destroy(&attr);

    if (err != 0) handle_error_en(err, "pthread_attr_destroy");

    free(client);

    for (int i = 0; i < client_info_count; i++) {
        free(cinfo_arr[i]);
    }

    free(cinfo_arr);

    if (close(socket_descr) == -1) handle_error_en(-1, "close");

    return 0;
}
