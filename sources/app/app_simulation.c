/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 21:42:53 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:25:25 by vlundaev         ###   ########.fr       */
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

/* rollback_threads_fail:
*	Rollback handler for thread creation failure:
*	- sets sim_stop to true (so running threads stop their loop)
*	- joins all already created philosopher threads
*	- prints error message and frees resources via error_failure()
*/
static bool	rollback_threads_fail(t_table *table, unsigned int started)
{
	set_sim_stop_flag(table, true);
	join_philo_threads(table, started);
	return (error_failure(STR_ERR_THREAD, NULL, table));
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
*	Sets a common start_time slightly in the future and launches all threads.
*	All threads wait until start_time via sim_start_delay(),
*	so timestamps are aligned.
*
*	If any thread creation fails:
*	- sim_stop is set to true
*	- already created philosopher threads are joined
*	- resources are freed and an error is printed
*/
bool	start_simulation(t_table *table)
{
	unsigned int	started;

	table->start_time = get_time_in_ms() + (table->nb_philos * 2 * 10);
	started = start_philo_threads(table);
	if (started != table->nb_philos)
		return (rollback_threads_fail(table, started));
	if (!start_death_watcher_thread(table))
		return (rollback_threads_fail(table, table->nb_philos));
	return (true);
}
