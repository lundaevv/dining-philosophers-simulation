/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 12:27:03 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/27 03:20:31 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* init_forks:
*	Allocates and initializes an array of fork mutexes.
*	Returns NULL on failure (and frees partially initialized forks).
*/
pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (error_return_null(STR_ERR_MALLOC, NULL, NULL));
	i = 0;
	while (i < (int)table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			print_msg(STR_ERR_MUTEX, NULL, EXIT_FAILURE);
			return (rollback_forks(forks, i));
		}
		i++;
	}
	table->forks_inited = table->nb_philos;
	return (forks);
}
