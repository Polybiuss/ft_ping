#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netdb.h>
# include <netinet/in.h>

# define NAME_PROG "ft_ping"
# define MISS_H_OPE "missing host operand"

// -v -? -f -l -n -W -w -p -r -s -T

// --verbose --help --flood --preload --numeric --timeout
// --linger --pattern --ignore-routing --size --tos 
// --ttl --ip-timestamp
typedef struct t_ping{
    char *dns;
    char *ip;
    uint8_t verbose;
    uint8_t flood;
    uint8_t preload;
    uint16_t *preload_number;
    uint8_t numeric;
    uint8_t timeout;
    uint16_t *timeout_number;
    uint8_t linger;
    uint16_t *ligner_number;
    uint8_t pattern;
    uint16_t *pattern_pattern;
    uint8_t ignore_routing;
    uint8_t size;
    uint16_t *size_number;
    uint8_t tos;
    uint8_t *tos_number;
    uint8_t ttl;
    uint8_t *ttl_number;
    uint8_t ip_timestamp;
    uint8_t ip_timestamp_flag;
} s_ping;

#endif