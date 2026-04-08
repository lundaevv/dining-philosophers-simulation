/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 23:03:29 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:20:32 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* take_forks:
*	Locks both forks for the philosopher in the predefined order.
*	Prints "has taken a fork" after each successful lock.
*
*	If sim_stop becomes true after taking the first or second fork,
*	we release already held forks to avoid deadlocks and return false.
*/
bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	if (has_simulation_stopped(philo->table) == true)
	{
		pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
		return (false);
	}
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	write_status(philo, false, GOT_FORK_2);
	if (has_simulation_stopped(philo->table) == true)
	{
		pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
		pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
		return (false);
	}
	return (true);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
}

void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
}

void	increment_times_ate(t_philo *philo)
{
	if (has_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
}
