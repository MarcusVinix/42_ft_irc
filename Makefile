SRC_DIR = ./ ./srcs ./srcs/server
OBJ_DIR = ./obj
INC_DIR = -I /. -I ./includes

SRC = ft_irc.cpp IrcServer.cpp
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
INC = ft_irc.hpp IrcServer.hpp

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
	./$(NAME) 3000 123

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

.PHONY: all clean fclean re
