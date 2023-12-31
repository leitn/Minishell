# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/28 15:18:14 by hedubois          #+#    #+#              #
#    Updated: 2023/11/30 00:00:33 by hedubois         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######  VARIABLES  ######

NAME		= minishell
SRC_PATH	= ./srcs/
INCL_PATH	= ./include/
BIN_PATH	= ./bin/

SRCS	=	main.c \
			builtin/bltn_cd.c \
			builtin/bltn_echo.c \
			builtin/bltn_env.c \
			builtin/bltn_exit.c \
			builtin/bltn_pwd.c \
			builtin/bltn_unset.c \
			builtin/bltn_export.c \
			builtin/bltn_export2.c \
			builtin/bltn_utils.c \
			env/env.c \
			env/env_utils.c \
			execution/start_exec.c \
			execution/exec_utils.c \
			execution/fds_utils.c \
			init_and_free/bin_utils.c \
			init_and_free/free_utils.c \
			init_and_free/free_utils2.c \
			init_and_free/init_utils.c \
			init_and_free/init_utils2.c \
			init_and_free/shlvl_utils.c \
			lexer/lexer_utils1.c \
			lexer/lexer_utils2.c \
			lexer/lexer.c \
			parser/parse_fds.c \
			parser/redir.c \
			parser/parse_utils1.c \
			parser/parse_utils2.c \
			parser/parse_utils3.c \
			parser/parse_utils4.c \
			parser/parse_utils5.c \
			parser/parser.c \
			signals/signals_inparent.c \
			signals/signals_inchildren.c \
			signals/signals_inhd.c \
			utils/diverses_utils.c \
			utils/error_utils.c \
			utils/ft_split.c \
			utils/libft.c \

OBJS	= $(addprefix $(BIN_PATH), $(SRCS:.c=.o))

DEPS	= $(OBJS:.o=.d)

CC		= cc

CFLAGS	= -Wall -Werror -Wextra -g3 -MMD -MP -I$(INCL_PATH)

LDFLAGS = -lreadline

RM		= rm -rf

######  RULES  ######

all: $(NAME)

$(BIN_PATH)%.o: $(SRC_PATH)%.c
	@ mkdir -p $(dir $@)
#	@ echo "Compiling $<"
	@ $(CC) $(CFLAGS) -c $< -o $@
#	@ echo "Compiling in $@"

$(NAME): $(OBJS) $(LDFLAGS)
#	@ echo "NAME en cours"
	@ $(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@ clear
	@ toilet Minishell Compiled! -F gay -f future

tclean:
	@ $(RM) *.t

clean: tclean
#	@ make clean --no-print-directory -C $(LDFLAGS)
	@ $(RM) $(BIN_PATH)
	@ toilet Cleaned! -F gay -f future

fclean: clean
#	@ make fclean --no-print-directory -C $(LDFLAGS)
	@ $(RM) $(NAME)
	@ clear
	@ toilet Fully cleaned! -F gay -f future

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
