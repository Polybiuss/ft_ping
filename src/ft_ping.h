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

# define NAME_PROG "ft_ping"
# define MISS_H_OPE "missing host operand"

// -v -? -f -l -n -W -w -p -r -s -T

// --verbose --help --flood --preload --numeric --timeout
// --linger --pattern --ignore-routing --size --tos 
// --ttl --ip-timestamp
typedef struct t_ping{
    char *dns;
    char
}s_ping;

#endif