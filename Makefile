# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbenmesb <mbenmesb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/23 23:32:52 by mbenmesb          #+#    #+#              #
#    Updated: 2021/08/23 23:33:07 by mbenmesb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = clang

INCLUDE_DIR	=	.
SRC_DIR	=	src
SRCS	=	pipex.c \
			create_fork.c \
			update_free_memory.c \
			get_path.c \
			display_error.c \
			utils.c \
			utils2.c

OBJS = $(SRCS:.c=.o)

RM = rm -f

CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR) 

all:		$(NAME)

.c.o:
		$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS)
	$(CC) -o $@ $^

clean:
			$(RM) $(OBJS) 

fclean:		clean
			$(RM) $(NAME)

re:		fclean all

.PHONY:		clean fclean re all 

