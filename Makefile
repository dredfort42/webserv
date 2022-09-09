NAME		= 	webserv

HDRS		=	Config/Parser.hpp\
				Config/ConfigStruct.hpp\
				HTTP/HTTPparser.hpp\
				HTTP/HTTPstruct.hpp\
				HTTP/HTTPResponse.hpp\
				Server/Service/Service.hpp\
				Server/Connection/Connection.hpp\
				Server/Server.hpp\
#				Server/Acceptor.hpp					Server/Handler.hpp			Server/Responder.hpp\
#                Server/Client.hpp					Server/Service.hpp   		Server/WebServer.hpp

SRCS 		=	Config/Parser.cpp\
				Config/ConfigStruct.cpp\
				HTTP/HTTPparser.cpp\
				HTTP/HTTPstruct.cpp\
				HTTP/HTTPResponse.cpp\
				main.cpp\
				Server/Service/Service.cpp\
				Server/Connection/Connection.cpp\
				Server/Server.cpp\
				Server/Acceptor.cpp\
				Server/Handler.cpp\
				Server/Processor.cpp\
				Server/Responder.cpp\
				Server/Terminator.cpp\
#				Server/Acceptor.cpp		Server/Handler.cpp			Server/Responder.cpp\
#                Server/Client.cpp		Server/Service.cpp			Server/WebServer.cpp\


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
