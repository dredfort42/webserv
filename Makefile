NAME		= 	webserv
HDRS		=	Server/Server.hpp Server/WebServer.hpp\
				Network/Binding.hpp Network/Connecting.hpp Network/Listening.hpp Network/Socket.hpp
SRCS 		=	Server/Server.cpp Server/WebServer.cpp\
				Network/Binding.cpp Network/Connecting.cpp Network/Listening.cpp Network/Socket.cpp


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
