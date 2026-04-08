/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:27:42 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:03:56 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* init_philosophers:
*	Allocates and initializes philosopher structures.
*	Each philo has its own meal_time_lock to protect last_meal/times_ate.
*/
t_philo	**init_philosophers(t_table *table)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo *) * table->nb_philos);
	if (!philos)
		return (error_return_null(STR_ERR_MALLOC, NULL, table));
	i = 0;
	while (i < (int)table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (rollback_philos(philos, i));
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		philos[i]->last_meal = 0;
		philos[i]->table = table;
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
		{
			free(philos[i]);
			return (rollback_philos(philos, i));
		}
		i++;
		table->philos_inited = i;
	}
	return (philos);
}
