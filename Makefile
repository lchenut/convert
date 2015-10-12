# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lchenut <lchenut@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/10/12 10:07:21 by lchenut           #+#    #+#              #
#    Updated: 2015/10/12 16:15:27 by lchenut          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= convert

ALL_CFLAGS	= -Wall -Wextra -Werror -O3
CC			= gcc
RM			= rm -rf

OBJS_DIR	= objs
SRCS_DIR	= srcs
INCS_DIR	= incs

SRCS		= convert.c		\
			  cv_init.c

S			= $(addprefix $(SRCS_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJS		= $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(ALL_CFLAGS) -I$(INCS_DIR) -I$(LIBFT)/$(INCS_DIR) -c $< -o $@
	@echo "\033[A\033[K\033[A"

clean:
	$(RM) $(OBJS)	

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(OBJS): | $(OBJS_DIR)

$(OBJS_DIR):
	mkdir $(OBJS_DIR)
