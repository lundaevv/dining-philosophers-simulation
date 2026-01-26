/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 20:42:45 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:02:22 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* set_sim_stop_flag:
*	Sets sim_stop flag under mutex.
*	Used to stop all philosopher threads at once.
*/
void	set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
	table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

/* kill_philo:
*	Checks if the philosopher exceeded time_to_die since last_meal.
*	Called while meal_time_lock is held.
*	If yes: sets sim_stop and prints death message.
*/
static bool	kill_philo(t_philo *philo, time_t now)
{
	if ((now - philo->last_meal) >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		write_status(philo, true, DIED);
		return (true);
	}
	return (false);
}

/* still_hungry:
*	Returns true if philosopher i has not eaten enough times yet.
*	If must_eat_count == -1, there is no "all ate enough" condition.
*/
static bool	still_hungry(t_table *table, unsigned int i)
{
	if (table->must_eat_count == -1)
		return (false);
	if (table->philos[i]->times_ate < (unsigned int)table->must_eat_count)
		return (true);
	return (false);
}

/* check_philo:
*	Checks one philosopher under meal_time_lock:
*	- death condition
*	- hunger status (used for finish condition)
*/
static bool	check_philo(t_table *table, unsigned int i, bool *hungry)
{
	time_t	now;

	pthread_mutex_lock(&table->philos[i]->meal_time_lock);
	now = get_time_in_ms();
	if (kill_philo(table->philos[i], now))
	{
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		return (true);
	}
	if (still_hungry(table, i))
		*hungry = true;
	pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
	return (false);
}

/* end_condition_reached:
*	Returns true if the simulation must stop:
*	- someone died
*	- everyone ate enough times (when must_eat_count is set)
*/
bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			hungry;

	hungry = false;
	i = 0;
	while (i < table->nb_philos)
	{
		if (check_philo(table, i, &hungry))
			return (true);
		i++;
	}
	if (table->must_eat_count != -1 && hungry == false)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}
