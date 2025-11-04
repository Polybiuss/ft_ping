NAME = ft_ping
LIB = libft.a
LIB_DIR = libft
SRC_DIR = src
SRC_PAR = $(SRC_DIR)/parsing

SRC = $(SRC_DIR)/main.c	\
	$(SRC_PAR)/valid_host.c	\
	$(SRC_PAR)/valid_arg.c	\


OBJ_DIR = obj

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
GREEN = \033[0;32m
CYAN = \033[0;36m
RESET = \033[0m
GRAY = \033[90m

SUCCESS = $(GREEN)[SUCCESS]$(RESET)
INFO = $(GRAY)[INFO]$(RESET)

all : $(NAME)

$(OBJ_DIR):
	@if [ ! -d $(OBJ_DIR) ]; then \
		echo "$(INFO) create $(OBJ_DIR)...$(GRAY)";	\
		mkdir -p $(OBJ_DIR); \
		echo "$(SUCCESS) $(OBJ_DIR) done !"; \
	fi

$(LIB):
	@if [ -d ./$(LIB_DIR) ]; then \
		echo "$(INFO) $(LIB_DIR) already exist";	\
	else	\
		echo "$(INFO) git clone Polybft in directory $(LIB_DIR)";	\
		git clone https://github.com/Polybiuss/libft.git $(LIB_DIR);	\
		echo "$(INFO) Make $(LIB)...$(GRAY)";	\
		make -sC $(LIB_DIR); \
		echo "$(SUCCESS) MAKE done !";	\
	fi

$(OBJ_DIR)/%.o: %.c | $(LIB)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LIB_DIR)/$(LIB) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)

fclean: clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re