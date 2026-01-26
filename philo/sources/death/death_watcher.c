/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_watcher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 21:45:12 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 21:45:22 by vlundaev         ###   ########.fr       */
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
*	- if a philosopher died (time since last meal >= time_to_die)
*	- if all philosophers ate enough times (optional argument)
*
*	It runs until end_condition_reached() returns true.
*/
void	*death_watcher(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	set_sim_stop_flag(table, false);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
