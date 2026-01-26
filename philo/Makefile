# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 12:51:45 by vlundaev          #+#    #+#              #
#    Updated: 2026/01/26 22:59:18 by vlundaev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo
CC		= cc
CFLAGS	= -Werror -Wall -Wextra -pthread

SRC_PATH = sources/
OBJ_PATH = objects/

SRC =  app/app_main.c \
       app/app_simulation.c \
       parse/parse_args.c \
       death/death_watcher.c \
       death/death_checks.c \
       init/init_table.c \
       init/init_mutex.c \
       init/init_forks.c \
       init/init_philos.c \
       init/init_rollback.c \
       init/init_fork_assign.c \
       output/output_status.c \
       output/output_errors.c \
       cleanup/cleanup_table.c \
       philo/philo_routine.c \
       philo/philo_routine_helpers.c \
       time/time_clock.c \
       time/time_sleep.c

SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean