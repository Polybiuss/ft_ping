#include "../libft/libft.h"
#include "ft_ping.h"

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc == 1){
        printf("%s: %s\n", NAME_PROG, MISS_H_OPE);
    }
    // for (int i = 0; i < argc; i++)
    // {
    //     printf("%d: %s\n", i, argv[i]);
    // }
    else if (argc == 2) {
        // struct hostent *host_info;
        // host_info = gethostbyname(argv[1]);
        // struct in_addr **addr_list;
        // if (!host_info){
        //     printf("Empty\n");
        // }else {
        //     printf("it's work \n");
        //     addr_list = (struct in_addr **) host_info->h_addr_list;
        //     for (int i = 0; addr_list[i]; i++)
        //     {
        //         printf("%s\n", inet_ntoa(*addr_list[i]));
        //     }
        struct in_addr addr;
        inet_aton(argv[1], &addr);
        struct hostent* host_info = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
        if (!host_info){
            printf("empty\n");
        } else {
            printf("work\n");
            printf("%s\n", host_info->h_name);

            //inet_pton() permet de savoir si ce le char * donne est une addresse ip
        }
        }
    return (0);
}