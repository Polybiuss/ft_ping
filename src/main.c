#include "../libft/libft.h"
#include "ft_ping.h"

int main(int argc, char *argv[]) {
    (void)argv;
    if (argc == 1){
        printf("%s: %s\n", NAME_PROG, MISS_H_OPE);
    }
    else {
        check_args(argc - 1, argv + 1);
        }
    return (0);
}