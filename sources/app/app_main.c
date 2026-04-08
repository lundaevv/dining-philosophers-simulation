/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:24:48 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:49:54 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* stop_simulation:
*	Joins all created threads and cleans resources.
*	Death watcher exists only when there are > 1 philosophers.
*/
static void	stop_simulation(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->death_watcher, NULL);
	cleanup_table(table);
}

/* main:
*	Entry point:
*	- validates input
*	- initializes table and mutexes
*	- starts threads
*	- waits until simulation ends
*/
int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (print_msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	table = init_table(ac, av);
	if (!table)
		return (EXIT_FAILURE);
	if (table->must_eat_count == 0)
	{
		cleanup_table(table);
		return (EXIT_SUCCESS);
	}
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
