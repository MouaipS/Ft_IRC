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
          Commands/CmdJoin.cpp \
          Commands/CmdNick.cpp \
          Commands/CmdPass.cpp \
          Commands/CmdPrivmsg.cpp \
          Commands/CmdUser.cpp \
          Commands/CmdPart.cpp 
          
OBJ = $(SOURCES:.cpp=.o)
OBJDIR = objects
OBJECTS = $(addprefix $(OBJDIR)/, $(OBJ))

CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98 -I./ -ICommands/

CXX = c++

ORANGE = \033[38;5;208m
RED    = \033[31m
GREEN  = \033[32m
RESET  = \033[0m

#-----------------------------------------#

all: $(NAME)
	@echo "$(GREEN)Done!$(RESET)"

$(NAME): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@if echo "$<" | grep -q "^Commands/"; then \
		echo "$(RED)Commands compiling : $<$(RESET)"; \
	else \
		echo "$(ORANGE)Server compiling : $<$(RESET)"; \
	fi
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR) $(OBJDIR)/Commands

run: all
	@./ircserv 6667 oui
	@./ircserv 6668 oui

fclean: clean
	@rm -f $(NAME)

clean:
	@rm -rf $(OBJDIR)

re: fclean all

.PHOONY: re fclean clean all done
