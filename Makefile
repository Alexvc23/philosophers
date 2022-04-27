
#                   _         __ _ _              _     _ _
#   _ __ ___   __ _| | _____ / _(_) | ___   _ __ | |__ (_) | ___
#  | '_ ` _ \ / _` | |/ / _ \ |_| | |/ _ \ | '_ \| '_ \| | |/ _ \
#  | | | | | | (_| |   <  __/  _| | |  __/ | |_) | | | | | | (_) |
#  |_| |_| |_|\__,_|_|\_\___|_| |_|_|\___| | .__/|_| |_|_|_|\___/
#                                          |_|

# ─── VARIABLES ───────────────────────────────────────────────────
	
V_CC := @echo "Building $@..."; $(CC)
AT := 
CC := gcc
EXEC			:= philo 
DIR_SRCS		:= srcs
FLAGS			:= -g -Wall -Werror -Wextra -pthread -fsanitize=thread
DIR_OBJS		:= objs
DIR_INCS		:= includes
FILE_INCS		:= philosophers.h
SRCS = 			$(addprefix $(DIR_SRCS)/,$(C_FILES))
OBJS = 			$(addprefix $(DIR_OBJS)/,$(O_FILES))
INCS = 			$(addprefix $(DIR_INCS)/,$(FILE_INCS))

# ─── O FILES ───────────────────────────────────────────────────────
	
O_FILES = $(C_FILES:.c=.o)

# ─── C FILES ───────────────────────────────────────────────────────
	
C_FILES	=	philosophers.c tools.c ft_atoi.c \
			end_of_threads.c thread_launch.c

# ─── COLORS ────────────────────────────────────────────────────────
	
ERASE	=	\033[2K\r
GREY	=	\033[30m
RED		=	\033[31m
GREEN	=	\033[32m
YELLOW	=	\033[33m
BLUE	=	\033[34m
PINK	=	\033[35m
CYAN	=	\033[36m
WHITE	=	\033[37m
BOLD	=	\033[1m
UNDER	=	\033[4m
SUR		=	\033[7m
END		=	\033[0m

# ─── RULES ──────────────────────────────────────────────────────────
	
all: $(EXEC)
	$(AT) chmod 777 $<

# Executable creation mandatory part 
$(EXEC): $(OBJS) $(INCS) Makefile
	$(AT) $(CC) $(FLAGS) $(OBJS) -o $(EXEC)
	@printf "$(ERASE)$(ERASE)$(BLUE)> Creating:$(BOLD)$(CYAN) $@ $(END)\n"


# Mandatory objs creation part 
$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c Makefile $(INCS) | $(DIR_OBJS)
	$(AT) $(CC) -I$(DIR_INCS) $(FLAGS) -c $< -o $@ 
	@printf "$(ERASE)$(ERASE)$(BLUE)> Creating:$(RED) $@ $(END)\n"

$(DIR_OBJS):
	$(AT) mkdir -p $@

clean:
	@echo Removing object files
	$(AT)-rm -rf $(DIR_OBJS)
fclean: clean
	@echo Removing application
	$(AT)-rm -f $(EXEC)
	@printf "$(ERASE)$(ERASE)$(BLUE)> Deleted : $(RED)$(EXEC)$(END)\n"

re: fclean all	

.PHONY: all clean fclean
.DEFAULT: all