NAME = ircserv

SOURCES = main.cpp \
		  Server.cpp \
		  User.cpp \
		  Channel.cpp \
		  Message.cpp \
		  ACommand.cpp \

OBJ = $(SOURCES:.cpp=.o)
OBJDIR = objects
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJ))

CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98

CXX = c++

#-----------------------------------------#

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

fclean: clean
	rm -f $(NAME)

clean:
	rm -rf $(OBJDIR)

re: fclean all

.PHOONY: re fclean clean all
