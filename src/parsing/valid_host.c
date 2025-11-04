#include "../../libft/libft.h"
#include "../ft_ping.h"

void is_valid_host(char *host){
    uint32_t buff;
    // buff = NULL;
    // ft_bzero(buff, 4);
    if (inet_pton(AF_INET, host, &buff) > 0) {
        struct in_addr addr;
        inet_aton(host, &addr);
        struct hostent* host_info = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
        if (!host_info){
            printf("empty ip\n");
        } else {
            printf("correct ip : %s\n", host_info->h_name);
        }
    } else {
        struct hostent *host_info;
        host_info = gethostbyname(host);
        struct in_addr **addr_list;
        if (!host_info) {
            printf("incorrect dns\n");
        } else {
            printf("correct dns :\n");
            addr_list = (struct in_addr **)host_info->h_addr_list;
            for (int i = 0; addr_list[i]; i++)
            {
                printf("%s\n", inet_ntoa(*addr_list[i]));
            }
        }
    }
}