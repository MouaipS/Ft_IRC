NAME = ircserv
NAME_BONUS = $(NAME)_bonus

#-----------------------------------------#
# Sources

SRC_manda = manda/Server.cpp \
            manda/User.cpp \
            manda/Channel.cpp \
            manda/Utils.cpp

SRC_bonus = bonus/Server.cpp \
            bonus/User.cpp \
            bonus/Channel.cpp \
            bonus/Utils.cpp \
            bonus/Commands/CmdBlackJack.cpp

SRC_shared = shared/main.cpp \
             shared/ICommand.cpp \
             shared/Exception.cpp \
             shared/Commands/CmdKick.cpp \
             shared/Commands/CmdInvite.cpp \
             shared/Commands/CmdTopic.cpp \
             shared/Commands/CmdMode.cpp \
             shared/Commands/CmdJoin.cpp \
             shared/Commands/CmdNick.cpp \
             shared/Commands/CmdPass.cpp \
             shared/Commands/CmdPrivmsg.cpp \
             shared/Commands/CmdUser.cpp \
             shared/Commands/CmdPart.cpp

#-----------------------------------------#
# Objects

OBJDIR = objs
OBJDIR_BONUS = objs_bonus

OBJ_shared = $(addprefix $(OBJDIR)/, $(SRC_shared:.cpp=.o))
OBJ_manda  = $(addprefix $(OBJDIR)/, $(SRC_manda:.cpp=.o))
OBJ_bonus  = $(addprefix $(OBJDIR_BONUS)/, $(SRC_bonus:.cpp=.o))

#-----------------------------------------#
# Compiler

CXX = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98

SHARED_INC = -I./shared -I./shared/Commands/
MANDA_INC = -I./manda 
BONUS_INC = -I./bonus -I./bonus/Commands/

#-----------------------------------------#
# Colors

ORANGE = \033[38;5;208m
RED    = \033[31m
GREEN  = \033[32m
RESET  = \033[0m

#-----------------------------------------#
# Rules

all: $(NAME)
	@echo "$(GREEN)Done!$(RESET)"

bonus: $(NAME_BONUS)
	@echo "$(GREEN)Done!$(RESET)"

$(NAME): $(OBJ_shared) $(OBJ_manda)
	@$(CXX) $(CXXFLAGS) $(SHARED_INC) $(MANDA_INC) $^ -o $@

$(NAME_BONUS): $(OBJ_shared) $(OBJ_bonus)
	@$(CXX) $(CXXFLAGS) $(SHARED_INC) $(BONUS_INC) $^ -o $(NAME_BONUS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@if echo "$<" | grep -q "Commands/"; then \
		echo "$(GREEN)Commands compiling : $<$(RESET)"; \
	else \
		echo "$(ORANGE)Server compiling : $<$(RESET)"; \
	fi
	@$(CXX) $(CXXFLAGS) $(SHARED_INC) $(MANDA_INC) -c $< -o $@

$(OBJDIR_BONUS)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@if echo "$<" | grep -q "Commands/"; then \
		echo "$(GREEN)Commands compiling : $<$(RESET)"; \
	else \
		echo "$(ORANGE)Server compiling : $<$(RESET)"; \
	fi
	@$(CXX) $(CXXFLAGS) $(SHARED_INC) $(BONUS_INC) -c $< -o $@


clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

bonus_clean:
	@rm -rf $(OBJDIR_BONUS)

bonus_fclean: bonus_clean
	@rm -f $(NAME_BONUS)

re: fclean all

re_bonus: bonus_fclean bonus

.PHONY: all bonus clean fclean re re_bonus bonus bonus_fclean bonus_clean
