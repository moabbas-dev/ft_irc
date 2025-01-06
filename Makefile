NAME = ircserv

SRCS = 	srcs/Server.cpp		\
		srcs/Client.cpp 	\
		srcs/Channel.cpp 	\
		srcs/Cmd.cpp		\
		srcs/commands/JOIN.cpp   	\
		srcs/commands/NICK.cpp		\
		srcs/commands/PART.cpp		\
		srcs/commands/PASS.cpp		\
		srcs/commands/PING.cpp		\
		srcs/commands/PRIVMSG.cpp	\
		srcs/commands/USER.cpp		\
		srcs/commands/KICK.cpp		\
		srcs/commands/INVITE.cpp		\
		srcs/commands/TOPIC.cpp		\
		srcs/commands/QUIT.cpp		\
		srcs/commands/MODE.cpp		\
		srcs/Errors.cpp		\
		srcs/StringUtils.cpp	\
		srcs/commands/ModeUtils.cpp		\
		main.cpp			\
# SRCS = srcs/main_test.cpp

OBJDIR = objs
OBJCS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

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