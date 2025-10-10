#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define PACKET_SIZE 56

uint16_t icmp_checksum(void *buf, int len){
    uint16_t *data = buf;
    uint32_t sum = 0;
    uint16_t res;

    while (len > 1){
        sum += *data;
        data++;
        len -= 2;
    }

    if (len == 1)
        sum += (uint8_t)*data;
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    res = ~sum;
    return res;
}

int main(void){
    int sock;
    uint8_t sendbuff[sizeof(struct icmphdr) + PACKET_SIZE];
    uint8_t recvbuf[1024]; // ???
    struct sockaddr_in dest;
    struct icmphdr *icmp;
    int packet_len;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0){
        perror("socket");
        exit(1);
    }

    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("8.8.8.8");
    memset(sendbuff, 0, sizeof(sendbuff));
    icmp = (struct icmphdr *)sendbuff;
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = htons(getpid() & 0xFFFF);
    icmp->un.echo.sequence = 0;

    for (int i = 0; i < PACKET_SIZE; ++i) sendbuff[sizeof(struct icmphdr) + i] = 0x45;

    packet_len = sizeof(struct icmphdr) + PACKET_SIZE;
    icmp->checksum = 0;
    icmp->checksum = icmp_checksum(&sendbuff, packet_len);

    ssize_t sent = sendto(sock, sendbuff, packet_len, 0, (struct sockaddr *)&dest, sizeof(dest));
    if (sent < 0){
        perror("sendto");
        close(sock);
        exit(1);
    }
    printf("Echo icmp sent\n");
    ssize_t receiv = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
    if (receiv < 0) {
        perror("recvfrom");
        close(sock);
        exit(1);
    }
    printf("receiv ECHO icmp reply\n");
    close(sock);
    return (1);
}