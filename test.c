#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/ip.h>
#include <stdint.h>

struct icmp_hdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
};

uint16_t icmp_checksum(void *buf, int len) {
    uint16_t *dbytes = buf;
    uint32_t sum = 0;
    uint16_t res;

    while (len > 1) {
        sum += *dbytes;
        dbytes++;
        len -= 2;
    }
    if (len == 1)
        sum += (uint8_t)*dbytes;
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    res = ~sum;
    return res;
}

double timeval_diff_ms(const struct timeval *start, const struct timeval *end) {
    double s = (double)(end->tv_sec - start->tv_sec);
    double us = (double)(end->tv_usec - start->tv_usec);
    return s * 1000.0 + us / 1000.0;
}

int main(int argc, char **argv){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ipv4-address>\n", argv[0]);
        return (1);
    }

    const char *target = argv[1];
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0){
        perror("socket");
        return (1);
    }

    struct sockaddr_in dst;
    memset(&dst, 0, sizeof(dst));
    dst.sin_family = AF_INET;
    if (inet_pton(AF_INET, target, &dst.sin_addr) != 1) {
        fprintf(stderr, "Invalid IPv4: %s\n", target);
        close(sock);
        return (1);
    }

    const uint8_t payload_len = 56;
    const int icmp_len = sizeof(struct icmp_hdr) + payload_len;
    uint8_t *packet = malloc(icmp_len);
    if (!packet) {
        perror("malloc");
        close(sock);
        return (1);
    }
    memset(packet, 0, icmp_len);
    struct icmp_hdr *icmp = (struct icmp_hdr *)packet;
    icmp->type = 8;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->id = htons(getpid() & 0xFFFF);
    icmp->sequence = htons(0);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    memcpy(packet + sizeof(struct icmp_hdr), &tv, sizeof(tv));
    for (int i = sizeof(tv); i < payload_len; ++i) {
        packet[sizeof(struct icmp_hdr) + i] = 'A' + (i % 26);
    }

    icmp->checksum = icmp_checksum(packet, icmp_len);

    ssize_t sent = sendto(sock, packet, icmp_len, 0,
            (struct sockaddr *)&dst, sizeof(dst));
    if (sent < 0){
        perror("sendto");
        free(packet);
        close(sock);
        return (1);
    }
    printf("Send %zd octets at %s\n", sent, target);

    uint8_t recvbuff[1500];
    struct sockaddr_in recv_addr;
    socklen_t addrlen = sizeof(recv_addr);

    // struct timeval tv_timeout;
    // tv_timeout.tv_sec = 2;
    // tv_timeout.tv_usec = 0;
    // fd_set readfds;
    // FD_ZERO(&readfds);
    // FD_SET(sock, &readfds);
    // int sel 

    ssize_t recvd = recvfrom(sock, recvbuff, sizeof(recvbuff), 0,
        (struct sockaddr *)&recv_addr, &addrlen);
    if (recvd < 0) {
        perror("recvfrom");
        free(packet);
        close(sock);
        return (1);
    } else {
        printf("received ECHO_REPLY\n");
    }
    free(packet);
    close(sock);
    return (0);
}

