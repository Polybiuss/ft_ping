#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PACKET_SIZE 64

uint16_t icmp_checksum(const void *buf, size_t len) {
    const uint8_t *bytes = buf;
    uint32_t sum = 0;

    while (len > 1) {
        sum += (uint16_t)bytes[0] << 8 | bytes[1];
        bytes += 2;
        len -= 2;
    }

    if (len == 1) {
        sum += (u_int16_t)bytes[0] << 8;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (uint16_t)(~sum);
}

int main() {
    int sock;
    struct sockaddr_in addr;
    char packet[PACKET_SIZE];
    struct icmphdr *icmp;
    socklen_t addr_len;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    memset(packet, 0 ,PACKET_SIZE);
    icmp = (struct icmphdr *)packet;
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = getpid() & 0xFFFF;
    icmp->un.echo.sequence = 1;
    icmp->checksum = 0;
    icmp->checksum = icmp_checksum(packet, sizeof(struct icmphdr));

    if (sendto(sock, packet, sizeof(struct icmphdr), 0,
            (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("sendto");
                exit(1);
            }
    printf("ICMP Echo request send to 8.8.8.8\n");
    
    addr_len = sizeof(addr);
    if (recvfrom(sock, packet, PACKET_SIZE, 0,
                (struct sockaddr *)&addr, &addr_len) < 0) 
                {
                    perror("recvfrom");
                    exit(1);
                }
        printf("ICMP Echo Reply received from 8.8.8.8\n");

    icmp = (struct icmphdr *)packet;
    uint16_t recv_checksum = icmp->checksum;
    icmp->checksum = 0;
    uint16_t calc_checksum = icmp_checksum(packet, sizeof(struct icmphdr));

    if (recv_checksum == calc_checksum) {
        printf("Checksum correct \n");
    } else {
        printf("Incorrect checksum \n");
    }

    close(sock);
    return (0);
}