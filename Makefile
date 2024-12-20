# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: temil-da <temil-da@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/02 11:51:28 by temil-da          #+#    #+#              #
#    Updated: 2024/12/03 19:14:58 by temil-da         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFLAGS = -Wall -Wextra -Werror

LFLAGS =  -lreadline -lcurses

CC = cc

LIBFT_DIR = includes/libft

LIBFT = $(LIBFT_DIR)/libft.a

SRC = execution/executor.c execution/builtin_commands.c execution/helper_functions.c execution/builtin_utils.c execution/init_redir.c \
	execution/builtin_commands_2.c execution/env_management.c  execution/env_management_2.c execution/cleanup.c execution/signal_handling.c\
	parsing/parser.c parsing/main_functions.c parsing/parser_utils.c parsing/struct_creation.c parsing/struct_freeing.c\
	parsing/main_functions_2.c lexing/lexer.c lexing/helper_functions.c lexing/lexer_utils.c lexing/print_error.c main.c
OBJ = $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean
fclean:
	rm -f $(OBJ)
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
re: fclean all
.PHONY: all clean fclean re