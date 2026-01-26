/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:23:36 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:01:53 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* cleanup_table:
*	Destroys all mutexes and frees all memory owned by table.
*	Safe to call with NULL.
*/
void	cleanup_table(t_table *table)
{
	if (!table)
		return ;
	destroy_mutexes(table);
	free_table(table);
}

/* free_table:
*	Frees all allocated memory in the table structure.
*	Returns NULL for convenient use in error handling.
*/
void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

/* destroy_mutexes:
*	Destroys only mutexes that were successfully initialized.
*	Uses forks_inited/philos_inited and init flags as protection.
*/
void	destroy_mutexes(t_table *table)
{
	unsigned int	i;

	if (!table)
		return ;
	i = 0;
	while (table->fork_locks != NULL && i < table->forks_inited)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		i++;
	}
	i = 0;
	while (table->philos != NULL && i < table->philos_inited)
	{
		if (table->philos[i] != NULL)
			pthread_mutex_destroy(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->write_lock_inited)
		pthread_mutex_destroy(&table->write_lock);
	if (table->sim_stop_lock_inited)
		pthread_mutex_destroy(&table->sim_stop_lock);
}

/* error_failure:
*	Prints an error message, frees resources and returns EXIT_FAILURE.
*/
int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		cleanup_table(table);
	print_msg(str, details, EXIT_FAILURE);
	return (EXIT_FAILURE);
}
