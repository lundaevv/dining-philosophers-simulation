/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 13:48:05 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:59:01 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

/******************************************************************************
*                                     Macros                                  *
******************************************************************************/

# define MAX_PHILOS	200
# define STR_MAX_PHILOS "200"

# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

/******************************************************************************
*                                 Structures                                  *
******************************************************************************/

typedef struct s_philo	t_philo;

/* t_table:
*	Fields overview:
*	- nb_philos, time_to_* and must_eat_count: input parameters
*	- start_time: common start timestamp for all threads
*	- sim_stop: global stop flag (protected by sim_stop_lock)
*	- fork_locks: array of mutexes (one per fork)
*	- write_lock: protects output printing
*	- death_watcher: monitoring thread
*	- philos: array of philosopher pointers
*	- *_inited flags: allow safe cleanup on partial init
*/
typedef struct s_table
{
	unsigned int		nb_philos;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	int					must_eat_count;
	time_t				start_time;
	bool				sim_stop;
	pthread_mutex_t		*fork_locks;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		sim_stop_lock;
	pthread_t			death_watcher;
	t_philo				**philos;
	unsigned int		forks_inited;
	unsigned int		philos_inited;
	int					write_lock_inited;
	int					sim_stop_lock_inited;
}	t_table;

/* t_philo:
*	One philosopher (one thread).
*
*	Fields overview:
*	- thread: pthread handle
*	- id: philosopher index (0..nb_philos-1)
*	- times_ate: number of meals eaten (protected by meal_time_lock)
*	- fork[2]: indices in table->fork_locks array
*	- meal_time_lock: protects last_meal and times_ate
*	- last_meal: timestamp of last meal start (ms)
*	- table: pointer to shared table
*/
typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}	t_philo;

/* t_status:
*	Used only for printing status messages via write_status().
*/
typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/******************************************************************************
*                           Function Prototypes                               *
******************************************************************************/

/* parse/parse_parse_args.c */
bool			is_valid_input(int ac, char **av);
int				integer_atoi(char *str);

/* init/init_table.c */
t_table			*init_table(int ac, char **av);

/* init/init_mutex.c */
int				init_global_mutexes(t_table *table);

/* init/init_philos.c */
t_philo			**init_philosophers(t_table *table);

/* init/init_forks.c */
pthread_mutex_t	*init_forks(t_table *table);

/* init/init_rollback.c */
pthread_mutex_t	*rollback_forks(pthread_mutex_t *forks, int last);
t_philo			**rollback_philos(t_philo **philos, int last);

/* init/init_fork_assign.c */
void			assign_all_forks(t_table *table);

/* philo/philo_routine.c */
void			*philosopher(void *data);

/* philo/philo_routine_helpers.c */
bool			take_forks(t_philo *philo);
void			drop_forks(t_philo *philo);
void			update_last_meal(t_philo *philo);
void			increment_times_ate(t_philo *philo);

/* time/time_clock.c */
time_t			get_time_in_ms(void);

/* time/time_sleep.c */
void			philo_sleep(t_table *table, time_t sleep_time);
void			sim_start_delay(time_t start_time);

/* output/output_status.c */
void			write_status(t_philo *philo, bool watcher, t_status status);

/* output/output_errors.c */
void			*error_return_null(char *str, char *details, t_table *table);
int				print_msg(char *str, char *detail, int exit_no);

/* death/death_watcher.c */
void			*death_watcher(void *data);
bool			has_simulation_stopped(t_table *table);

/* death/death_checks.c */
void			set_sim_stop_flag(t_table *table, bool state);
bool			end_condition_reached(t_table *table);

/* cleanup/cleanup_table.c */
int				error_failure(char *str, char *details, t_table *table);
void			*free_table(t_table *table);
void			destroy_mutexes(t_table *table);
void			cleanup_table(t_table *table);

/* app/app_simulation.c */
bool			start_simulation(t_table *table);

#endif