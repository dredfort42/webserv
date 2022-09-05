NAME		= 	webserv
HDRS		=	Server/Service.hpp Server/WebServer.hpp Config/Parser.hpp 
SRCS 		=	Server/Service.cpp Server/WebServer.cpp Config/Parser.cpp\
				Config/ConfigStruct.cpp main.cpp

OBJS		=	$(patsubst %.cpp, %.o,$(SRCS))

CC			= 	c++
FLAGS		= 	-Wall -Werror -Wextra -Wshadow
STD98		=	-std=c++98
LEAKS		=	-fsanitize=address -g

all : $(NAME)

$(NAME) 	: 	$(OBJS) $(HDRS)
				$(CC) $(FLAGS) $(STD98) $(LEAKS) $(OBJS) -o $@

%.o 		: 	%.cpp $(HDRS)
				$(CC) $(FLAGS) $(STD98) $(LEAKS) -c $< -o $@

clean 		:
				@rm -f $(OBJS)

fclean 		: 	clean
				@rm -f $(NAME)

re 			: 	fclean all

.PHONY 		: 	all clean fclean re
