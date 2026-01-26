/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 20:42:45 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:23:06 by vlundaev         ###   ########.fr       */
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
*
*	If the philosopher is dead:
*	- sets sim_stop to true (under sim_stop_lock)
*	- prints the "died" line via write_status(..., true, DIED)
*	Returns true if death condition is met.
*/
static bool	kill_philo(t_philo *philo, time_t now)
{
	if ((now - philo->last_meal) >= philo->table->time_to_die)
	{
		if (has_simulation_stopped(philo->table) == false)
		{
			set_sim_stop_flag(philo->table, true);
			write_status(philo, true, DIED);
		}
		return (true);
	}
	return (false);
}

/* has_hunger_condition:
*	Returns true if simulation has "everyone ate enough" finish condition.
*/
static bool	has_hunger_condition(t_table *table)
{
	if (table->must_eat_count == -1)
		return (false);
	return (true);
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
	if (has_hunger_condition(table))
	{
		if (table->philos[i]->times_ate < (unsigned int)table->must_eat_count)
			*hungry = true;
	}
	pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
	return (false);
}

/* end_condition_reached:
*	Checks global end conditions for the simulation.
*
*	Returns true when:
*	- at least one philosopher died (kill_philo sets sim_stop + prints "died")
*	- everyone ate enough times (when must_eat_count is enabled)
*
*	When "everyone ate enough" is reached, this function sets sim_stop to true.
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
	if (has_hunger_condition(table) && hungry == false)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}
