#include "../../libft/libft.h"
#include "../ft_ping.h"

int check_tiret(char *arg){
    int len = ft_strlen(arg);

    if (len <= 1) {
        return (0);
    } else {
        if (arg[0] == '-' && arg[1] == '-') {
            // printf("double-tiret\n");
            return (2);
        } else if (arg[0] == '-') {
            // printf("single-tiret\n");
            return (1);
        } else {
            return (0);
        }
    }
}

void check_args(int len, char *argv[]){
    for (int i = 0; i < len; i++)
    {
        printf("arg number : %d\n", i);
        switch (check_tiret(argv[i]))
        {
        case 0:
            printf("length too low or ip/dns or value of option\n");
            //need to define 
            break;
        case 1:
            printf("a single-tiret argument\n");
            //single characters
            break;
        case 2:
            printf("a double-tiret argument\n");
            //string
            break;    
        }
        // check_tiret(argv[i])
    }
    
}
