/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fork_assign.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 13:41:44 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:04:45 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* assign_all_forks:
*	Computes fork order per philosopher.
*	Even/odd philosophers take forks in different order to reduce deadlocks.
*/
void	assign_all_forks(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		table->philos[i]->fork[0] = table->philos[i]->id;
		table->philos[i]->fork[1] = (table->philos[i]->id + 1)
			% table->nb_philos;
		if (table->philos[i]->id % 2)
		{
			table->philos[i]->fork[0] = (table->philos[i]->id + 1)
				% table->nb_philos;
			table->philos[i]->fork[1] = table->philos[i]->id;
		}
		i++;
	}
}
