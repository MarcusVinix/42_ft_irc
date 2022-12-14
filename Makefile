# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 04:03:12 by Barney e Se       #+#    #+#              #
#    Updated: 2022/08/18 11:35:28 by Barney e Se      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = ./srcs ./srcs/classes
OBJ_DIR = ./obj
INC_DIR = -I ./includes -I ./srcs/classes

SRC = ft_irc.cpp IrcServer.cpp User.cpp Channel.cpp Command.cpp Utils.cpp
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
INC = ft_irc.hpp IrcServer.hpp User.hpp Channel.hpp Command.hpp Utils.hpp

vpath %.cpp $(SRC_DIR)
vpath %.hpp $(INC_DIR)

NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
RM = rm -rf

#_________________________/\__________________________#

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC_DIR) -o $(NAME) $(OBJ)
	@echo "\033[1;32m"
	@echo "/ ************************************ \\"
	@echo "|           $(NAME) Created            |"
	@echo "\\ ************************************ /"
	@echo "\033[0m"

$(OBJ_DIR)/%.o: %.cpp $(INC)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/server
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

#_________________________/\__________________________#

run: $(NAME)
	./$(NAME) 6667 123

run_val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 6667 123

#_________________________/\__________________________#

clean:
	$(RM) $(OBJ_DIR)
	@echo "\033[1;31m"
	@echo "/ ************************************ \\"
	@echo "|          Deleted .o files            |"
	@echo "\\ ************************************ /"
	@echo "\033[0m"

fclean: clean
	$(RM) $(NAME)
	@echo "\033[1;31m"
	@echo "/ ************************************ \\"
	@echo "|          Deleted executables         |"
	@echo "\\ ************************************ /"
	@echo "\033[0m"

#_________________________/\__________________________#

re: fclean all

git:
	@git add .
	@git status
	@git commit -m "$m"

.PHONY: all clean fclean re git
