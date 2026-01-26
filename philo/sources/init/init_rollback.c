/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rollback.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 14:23:02 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:03:37 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* rollback_forks:
*	Destroys and frees fork mutexes up to index "last - 1".
*	Used when fork initialization fails mid-way.
*/
pthread_mutex_t	*rollback_forks(pthread_mutex_t *forks, int last)
{
	int	i;

	i = 0;
	while (i < last)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	return (NULL);
}

/* rollback_philos:
*	Destroys and frees philosopher structures up to index "last - 1".
*	Used when philosopher allocation/init fails mid-way.
*/
t_philo	**rollback_philos(t_philo **philos, int last)
{
	int	i;

	i = 0;
	while (i < last)
	{
		pthread_mutex_destroy(&philos[i]->meal_time_lock);
		free(philos[i]);
		i++;
	}
	free(philos);
	return (NULL);
}
