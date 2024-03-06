#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define cntr 1000

#define handle_error_en(err, msg) \
    do {                          \
        errno = err;              \
        perror(msg);              \
        exit(EXIT_FAILURE);       \
    } while (0)

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

void *receive(void *arg) {
    int *server = (int *)arg;
    char msg[256];
    int ct = 0;

    while (1) {
        int err = recv(*server, msg, 255, 0);

        if (err > 0) {
            msg[256] = '\0';
            printf("%s", msg);
            ct++;

            if (ct > 2000) break;
        }

        if (err == 0) {
            break;
        }

        if (err == -1) handle_error_en(-1, "recv");
    }

    pthread_exit(0);
}

int main(int argc, char **argv) {
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

    memset(datagram, 0, 4096); /* zero out the buffer */

    // Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
    iph->id = htons(54321);  // Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;                     // Set to 0 before calculating checksum
    iph->saddr = inet_addr(source_ip);  // Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;

    iph->check = csum((unsigned short *)datagram, iph->tot_len >> 1);

    // TCP Header
    tcph->source = htons(1234);
    tcph->dest = htons(8000);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5; /* first and only tcp segment */
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons(5840); /* maximum allowed window size */
    tcph->check = 0; /* if you set a checksum to zero, your kernel's IP stack
           should fill in the correct checksum during transmission */
    tcph->urg_ptr = 0;
    // Now the IP checksum

    psh.source_address = inet_addr(source_ip);
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);

    memcpy(&psh.tcp, tcph, sizeof(struct tcphdr));

    tcph->check = csum((unsigned short *)&psh, sizeof(struct pseudo_header));

    system("clear");
    pthread_attr_t attr;
    int err = pthread_attr_init(&attr);

    if (err != 0) handle_error_en(err, "pthread_attr_init");

    int socket_descr = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_descr == -1) handle_error_en(-1, "socket");

    struct sockaddr_in server_sa;
    server_sa.sin_family = AF_INET;
    server_sa.sin_port = htons(1234);
    server_sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    char user_name[14];
    char txt[240], msg[256];

    printf("Username: ");
    scanf("%s", user_name);

    if (connect(socket_descr, (struct sockaddr *)&server_sa,
                sizeof(server_sa)) == -1)
        handle_error_en(-1, "connect");

    pthread_t thread;
    err = pthread_create(&thread, &attr, receive, &socket_descr);

    if (err != 0) handle_error_en(err, "pthread_create");

    int ctr = 0;

    while (1) {
        send(socket_descr, datagram, strlen(datagram), 0);
    }

    close(socket_descr);
}
