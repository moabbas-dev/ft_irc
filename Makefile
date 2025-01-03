NAME = ircserv

SRCS = 	main.cpp			\
		srcs/Server.cpp		\
		srcs/Client.cpp 	\
		srcs/Channel.cpp 	\
		srcs/Cmd.cpp		\
		srcs/JOIN.cpp   	\
		srcs/NICK.cpp		\
		srcs/PART.cpp		\
		srcs/PASS.cpp		\
		srcs/PING.cpp		\
		srcs/PRIVMSG.cpp	\
		srcs/USER.cpp		\
		srcs/KICK.cpp		\
		srcs/INVITE.cpp		\
		srcs/TOPIC.cpp		\
		srcs/MODE.cpp		\
		srcs/Errors.cpp		\
		srcs/QUIT.cpp		\

# SRCS = srcs/main_test.cpp

OBJDIR = objs
OBJCS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
CC = g++
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJCS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJCS) 

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJCS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re