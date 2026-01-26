/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 21:42:53 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 21:43:05 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* join_philo_threads:
*	Joins philosopher threads for "count" philosophers.
*	Used as rollback when thread creation fails.
*/
static void	join_philo_threads(t_table *table, unsigned int count)
{
	while (count > 0)
	{
		count--;
		pthread_join(table->philos[count]->thread, NULL);
	}
}

/* start_philo_threads:
*	Creates one thread per philosopher.
*	Returns the number of successfully created threads.
*/
static unsigned int	start_philo_threads(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			break ;
		i++;
	}
	return (i);
}

/* start_death_watcher_thread:
*	Creates monitoring thread that checks death/end condition.
*	For 1 philosopher, this thread is skipped.
*/
static bool	start_death_watcher_thread(t_table *table)
{
	if (table->nb_philos <= 1)
		return (true);
	if (pthread_create(&table->death_watcher, NULL, &death_watcher, table) != 0)
		return (false);
	return (true);
}

/* start_simulation:
*	Sets a future start_time and launches all threads.
*	If any thread creation fails:
*	- sim_stop is set to true
*	- already created threads are joined
*	- resources are freed
*/
bool	start_simulation(t_table *table)
{
	unsigned int	started;

	table->start_time = get_time_in_ms() + (table->nb_philos * 2 * 10);
	started = start_philo_threads(table);
	if (started != table->nb_philos)
	{
		pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = true;
		pthread_mutex_unlock(&table->sim_stop_lock);
		join_philo_threads(table, started);
		return (error_failure(STR_ERR_THREAD, NULL, table));
	}
	if (!start_death_watcher_thread(table))
	{
		pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = true;
		pthread_mutex_unlock(&table->sim_stop_lock);
		join_philo_threads(table, table->nb_philos);
		return (error_failure(STR_ERR_THREAD, NULL, table));
	}
	return (true);
}
