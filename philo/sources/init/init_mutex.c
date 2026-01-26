/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 19:11:22 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:04:13 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* init_global_mutexes:
*	Initializes fork mutexes and shared table mutexes.
*	- sim_stop_lock protects sim_stop flag
*	- write_lock protects output printing
*/
int	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (1);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	table->sim_stop_lock_inited = 1;
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	table->write_lock_inited = 1;
	return (0);
}
