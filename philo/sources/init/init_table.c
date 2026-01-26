/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:14:24 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:03:19 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* init_table_defaults:
*	Sets safe default values for table.
*	Important for cleanup because destroy/free depends on init flags.
*/
static void	init_table_defaults(t_table *table)
{
	table->start_time = 0;
	table->sim_stop = false;
	table->fork_locks = NULL;
	table->philos = NULL;
	table->forks_inited = 0;
	table->philos_inited = 0;
	table->write_lock_inited = 0;
	table->sim_stop_lock_inited = 0;
}

/* init_table_args:
*	Reads runtime parameters from argv.
*	must_eat_count = -1 means "no limit".
*/
static void	init_table_args(t_table *table, int ac, char **av)
{
	table->nb_philos = integer_atoi(av[1]);
	table->time_to_die = integer_atoi(av[2]);
	table->time_to_eat = integer_atoi(av[3]);
	table->time_to_sleep = integer_atoi(av[4]);
	table->must_eat_count = -1;
	if (ac == 6)
		table->must_eat_count = integer_atoi(av[5]);
}

/* init_table:
*	Creates the shared table and initializes all simulation resources.
*	Returns NULL on error.
*/
t_table	*init_table(int ac, char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (error_return_null(STR_ERR_MALLOC, NULL, NULL));
	init_table_args(table, ac, av);
	init_table_defaults(table);
	if (init_global_mutexes(table) != 0)
		return (NULL);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (error_return_null(STR_ERR_MALLOC, NULL, table));
	assign_all_forks(table);
	return (table);
}
