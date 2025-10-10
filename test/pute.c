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

int main(void) {
    uint8_t packet1[64];
    // uint8_t packet2[64];
    // memset(packet, 2, 64);
    printf("jbergos = %u\n", icmp_checksum(packet1, 64));
    printf("cauvray = %u\n", checksum(packet1, 64));
}