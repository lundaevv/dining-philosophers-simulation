/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 11:57:18 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:06:22 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* philo_sleep:
*	Sleeps for "sleep_time" milliseconds but checks sim_stop periodically.
*	This prevents threads from being stuck sleeping after simulation end.
*/
void	philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (has_simulation_stopped(table))
			break ;
		usleep(100);
	}
}

/* sim_start_delay:
*	Busy-wait loop that delays thread start until common start_time.
*	This aligns philosophers and death_watcher to the same time reference.
*/
void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		usleep(500);
}
