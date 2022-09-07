NAME		= 	webserv

HDRS		=	Config/Parser.hpp		Config/ConfigStruct.hpp\
				Server/Acceptor.hpp		Server/Handler.hpp			Server/Responder.hpp\
                Server/Client.hpp		Server/Service.hpp   		Server/WebServer.hpp\
				HTTP/HTTPparser.hpp		HTTP/HTTPstruct.hpp
SRCS 		=	Config/Parser.cpp		Config/ConfigStruct.cpp\
				Server/Acceptor.cpp		Server/Handler.cpp			Server/Responder.cpp\
                Server/Client.cpp		Server/Service.cpp			Server/WebServer.cpp\
				main.cpp				HTTP/HTTPparser.cpp			HTTP/HTTPstruct.cpp

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
