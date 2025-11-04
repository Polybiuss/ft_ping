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

# define CMD_SHORT "v?flnWwprst"
/*
cmd on 16bit: 
    - [0] verbose
    - [1] flood
    - [2] preload
    - [3] numeric
    - [4] timeout
    - [5] linger
    - [6] pattern
    - [7] ignore-routing
    - [8] size
    - [9] tos
    - [10] ttl
    - [11] ip-timestamp
*/
typedef struct t_ping{
    char *dns;
    char *ip;
    uint16_t cmd;
    uint16_t *preload_number;
    uint16_t *timeout_number;
    uint16_t *linger_number;
    uint16_t *pattern_pattern;
    uint16_t *size_number;
    uint8_t *tos_number;
    uint8_t *ttl_number;
    uint8_t ip_timestamp_flag;
} s_ping;

void is_valid_host(char *host);
void check_args(int len, char *argv[]);

#endif