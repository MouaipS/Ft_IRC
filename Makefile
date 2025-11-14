NAME = ircserv

SOURCES = main.cpp \
          Server.cpp \
          User.cpp \
          Channel.cpp \
          ICommand.cpp \
		  Exception.cpp \
		  Utils.cpp \
          Commands/CmdKick.cpp \
          Commands/CmdInvite.cpp \
          Commands/CmdTopic.cpp \
          Commands/CmdMode.cpp \
          Commands/CmdModeSub.cpp \
          Commands/CmdJoin.cpp \
          Commands/CmdNick.cpp \
          Commands/CmdPass.cpp \
          Commands/CmdPrivmsg.cpp \
          Commands/CmdUser.cpp \
          Commands/CmdPart.cpp \
          Commands/CmdSee.cpp 

OBJ = $(SOURCES:.cpp=.o)
OBJDIR = objects
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJ))

CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98 -I./ -ICommands/

CXX = c++

#-----------------------------------------#

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR) $(OBJDIR)/Commands

run: all
	./ircserv 6667 oui
	./ircserv 6668 oui

fclean: clean
	rm -f $(NAME)

clean:
	rm -rf $(OBJDIR)

re: fclean all

.PHOONY: re fclean clean all
