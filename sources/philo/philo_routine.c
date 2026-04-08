/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:58:56 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:25:39 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* philo_is_full:
*	Returns true if this philosopher already ate enough times.
*	Used to stop running routines when must_eat_count is reached.
*/
static bool	philo_is_full(t_philo *philo)
{
	bool	full;

	if (philo->table->must_eat_count == -1)
		return (false);
	pthread_mutex_lock(&philo->meal_time_lock);
	full = (philo->times_ate >= (unsigned int)philo->table->must_eat_count);
	pthread_mutex_unlock(&philo->meal_time_lock);
	return (full);
}

/* eat_sleep_routine:
*	One full cycle step:
*	1) lock both forks (mutexes)
*	2) print fork states + "is eating"
*	3) update last_meal timestamp (protected by meal_time_lock)
*	4) sleep time_to_eat
*	5) increment times_ate (protected, only if sim_stop is false)
*	6) unlock both forks
*	7) print "is sleeping"
*	8) sleep time_to_sleep
*/
static void	eat_sleep_routine(t_philo *philo)
{
	if (!take_forks(philo))
		return ;
	write_status(philo, false, EATING);
	update_last_meal(philo);
	philo_sleep(philo->table, philo->table->time_to_eat);
	increment_times_ate(philo);
	drop_forks(philo);
	write_status(philo, false, SLEEPING);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

/* think_routine:
*	Adds a controlled delay to reduce contention between philosophers.
*
*	The thinking time is computed as:
*	(time_to_die - time_since_last_meal - time_to_eat) / 2
*
*	Notes:
*	- The value can become negative and is clamped to 0.
*	- If silent == true and computed time is 0,
*	  we sleep 1ms to avoid tight loops.
*	- Long values are capped to reduce excessive idle waiting.
*	- If silent == false, prints "is thinking".
*/
static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(philo->table, time_to_think);
}

/* lone_philo_routine:
*	Special case for 1 philosopher:
*	He can take one fork but will never get a second one, so he dies.
*/
static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	philo_sleep(philo->table, philo->table->time_to_die);
	set_sim_stop_flag(philo->table, true);
	write_status(philo, true, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

/* philosopher:
*	Thread routine for each philosopher.
*	- initializes last_meal to start_time
*	- waits for global start delay
*	- handles special cases (0 meals required, 1 philosopher, time_to_die == 0)
*	- runs eat/sleep/think loop until:
*		- sim_stop becomes true
*		- or this philosopher reached must_eat_count
*/
void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->nb_philos == 1)
		return (lone_philo_routine(philo));
	else if (philo->id % 2)
		think_routine(philo, true);
	while (has_simulation_stopped(philo->table) == false)
	{
		if (philo_is_full(philo))
			break ;
		eat_sleep_routine(philo);
		if (philo_is_full(philo))
			break ;
		think_routine(philo, false);
	}
	return (NULL);
}
