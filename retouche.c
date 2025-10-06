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

#define RECV_BUF_SZ 1024
#define DATA_LEN 56

uint16_t icmp_checksum(void *buf, int len) {
    u_int16_t *bytes = buf;
    u_int32_t sum = 0;
	u_int16_t result;

    while (len > 1) {
        sum += *bytes;
        bytes++;
        len -= 2;
    }

    if (len == 1) {
        sum += *(uint8_t *)bytes;
    }
        sum = (sum >> 16) + (sum & 0xFFFF);
        sum += (sum >> 16);
        result = ~sum;
    return result;
}
uint16_t checksum(void *b, int len) {
	u_int16_t *buf = b;
	u_int32_t sum = 0;
	u_int16_t result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(uint8_t *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

int main (void) {
    int sock;
    struct sockaddr_in dest;
    uint8_t sendbuff[sizeof(struct icmphdr) + DATA_LEN];
    uint8_t recvbuf[RECV_BUF_SZ];
    struct icmphdr *icmp;
    int packet_len;
    int seq = 0;

    if (getuid() != 0) {
        fprintf(stderr, "Warning: raw sockets require toot (run with sudo)\n");
    }

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0){
        perror("socket");
        exit(1);
    }

    struct timeval tv = {1, 0};
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt SO_RCVTIMEO");
    }

    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    // if (inet_pton(AF_INET, "8.8.8.8", &dest.sin_addr) != 1) {
    //     fprintf(stderr, "inet_pton failed\n");
    //     exit(1);
    // }
    dest.sin_port = htons(0);
    dest.sin_addr.s_addr = inet_addr("8.8.8.8");
    memset(sendbuff, 0, sizeof(sendbuff));
    icmp = (struct icmphdr *)sendbuff;
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = htons(getpid() & 0xFFFF);
    icmp->un.echo.sequence = htons(seq);

    for (int i = 0; i < DATA_LEN; ++i) sendbuff[sizeof(struct icmphdr) + i] = 0xA5;

    packet_len = sizeof(struct icmphdr) + DATA_LEN;
    // printf("sendbuff %hhn, packet_len %d", sendbuff, packet_len);
    icmp->checksum = 0;
    icmp->checksum = checksum(&sendbuff, packet_len);

    struct timeval t_send, t_recv;
    gettimeofday(&t_send, NULL);
    ssize_t sent = sendto(sock, sendbuff, packet_len, 0,
            (struct sockaddr *)&dest, sizeof(dest));
        if (sent < 0) {
            perror("sendto");
            close(sock);
            exit(1);
        }
    printf("Echo Request sent (%zd octets)\n", sent);

    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    ssize_t r = recvfrom(sock, recvbuf, sizeof(recvbuf), 0,
            (struct sockaddr *)&from, &fromlen);
    if (r < 0) {
        if (errno = EAGAIN || errno == EWOULDBLOCK) {
            fprintf(stderr, "Timeout: no reponse received in delay imparted\n");
        } else {
            perror("recvfrom");
        }
        // close(sock);
        // exit(1);
    }
    gettimeofday(&t_recv, NULL);

    double rtt = (t_recv.tv_sec - t_send.tv_sec) * 1000.0
        + (t_recv.tv_usec - t_send.tv_usec) / 1000.0;
    
    if (r < (ssize_t)sizeof(struct iphdr) + (ssize_t)sizeof(struct icmphdr)) {
        fprintf(stderr, "PACKET recevied too small (%zd octets)\n", r);
        close(sock);
        exit(1);
    }
    struct iphdr *ip = (struct iphdr *)recvbuf;
    int iphdr_len = ip->ihl * 4;
    if (r < iphdr_len + (int)sizeof(struct icmphdr)) {
        fprintf(stderr, "Packet too small for contain ICMP\n");
        close(sock);
        exit(1);
    }

    struct icmphdr *recv_icmp = (struct icmphdr *)(recvbuf + iphdr_len);

    if (recv_icmp->type != ICMP_ECHOREPLY) {
        printf("Recevice ICMP type no-allowed: %d (code %d)\n", recv_icmp->type, recv_icmp->code);
    } else {
        uint16_t recv_id = ntohs(recv_icmp->un.echo.id);
        uint16_t recv_seq = ntohs(recv_icmp->un.echo.sequence);
        uint16_t our_id = getpid() & 0xFFFF;
        printf("Received ICMP ECHO_REPLY id=%u seq=%u from %s (rtt=%.3f ms)\n",
            recv_id, recv_seq, inet_ntoa(from.sin_addr), rtt);
        if (recv_id != our_id || recv_seq != seq) {
            printf("-> id/seq dont correspond (wanted id=%u seq=%u)\n",
            our_id, seq);
        } else {
            uint16_t orig_ck = recv_icmp->checksum;
            recv_icmp->checksum = 0;
            int icmp_len = r - iphdr_len;
            uint16_t calc_ck = checksum(recv_icmp, icmp_len);
            if (orig_ck == calc_ck) {
                printf("Checksum correct (0x%04x)\n", orig_ck);
            } else {
                printf("Checksum incorrect receive=0x%04x calculated=0x%04x\n",
                    orig_ck, calc_ck);
            }
        }
    }
    close (sock);
    return (0);
}