/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_watcher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 21:45:12 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:23:19 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* has_simulation_stopped:
*	Reads the shared sim_stop flag under mutex.
*	Used by every thread to know when to stop running.
*/
bool	has_simulation_stopped(t_table *table)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		r = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (r);
}

/* death_watcher:
*	Monitor thread that checks:
*	- if a philosopher died (time since last_meal >= time_to_die)
*	- if all philosophers ate enough times (optional must_eat_count condition)
*
*	The watcher starts monitoring only after the common start_time
*	(using sim_start_delay()).
*
*	It runs until:
*	- sim_stop becomes true
*	- or end_condition_reached() returns true (death or everyone ate enough)
*/
void	*death_watcher(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (has_simulation_stopped(table) == true)
			return (NULL);
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
