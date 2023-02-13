#####   COLORS   #####

END				= \033[0m

GREY			= \033[30m
RED				= \033[31m
GREEN			= \033[32m
YELLOW			= \033[33m
BLUE			= \033[34m
PURPLE			= \033[35m
CYAN			= \033[36m

HIGH_RED		= \033[91m

#####   COMMANDS   #####

CC				= c++
CFLAGS			= -Wall -Wextra -Werror -std=c++98

MD				= mkdir -p
RM				= rm -rf

#####   INFO   #####

NAME			= ircbot
PORT			= 6667
PASSWORD		= "test"
HOST			= "127.0.0.1"

#####   RESOURCES   #####

INCLUDE			= includes/
SRC_DIR			= srcs
OBJ_DIR			= objs

SRC_SUB_DIRS	= $(shell find $(SRC_DIR) -type d)
OBJ_SUB_DIRS	= $(SRC_SUB_DIRS:$(SRC_DIR)%=$(OBJ_DIR)%)

SRCS			= $(foreach DIR, $(SRC_SUB_DIRS), $(wildcard $(DIR)/*.cpp))
OBJS			= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)



#####   BASE RULES   #####

all: $(NAME)

$(NAME): $(OBJ_SUB_DIRS) $(OBJS)
	@ $(CC) $(CFLAGS) $(OBJS) -o $@
	@ echo "$(GREEN)[+] $(NAME) compiled$(END)"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@ $(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@
	@ echo "$(BLUE)[+] $@ compiled$(END)"

$(OBJ_SUB_DIRS):
	@ $(MD) $(OBJ_SUB_DIRS)
	@ echo "$(PURPLE)[+] $(SRC_DIR) remapped into $(OBJ_DIR) $(END)"


clean:
	@ $(RM) $(OBJ_DIR)
	@ echo "$(YELLOW)[+] $(OBJ_DIR) cleaned$(END)"

fclean: clean
	@ $(RM) $(NAME)
	@ echo "$(YELLOW)[+] $(NAME) fcleaned$(END)"

re: fclean all

bonus: all



#####   EXTRA RULES   #####

test: all
	clear
	@ ./$(NAME) $(HOST) $(PORT) $(PASSWORD)

run: test
rrun: fclean test

val: all
	valgrind --leak-check=full ./$(NAME) $(HOST) $(PORT) $(PASSWORD)
var: val



#####   PHONY   #####

.PHONY: all clean fclean re
