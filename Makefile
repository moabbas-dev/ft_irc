NAME = PmergeMe
SRCS = 	PmergeMe.cpp	\
		main.cpp		\

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