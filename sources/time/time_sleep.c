/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 11:57:18 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:24:05 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* philo_sleep:
*	Sleeps for approximately "sleep_time" milliseconds.
*	Implements a small polling loop with usleep(200) to:
*	- wake up early if sim_stop becomes true
*	- reduce oversleeping compared to one long usleep()
*/
void	philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;
	time_t	now;

	wake_up = get_time_in_ms() + sleep_time;
	while (true)
	{
		if (has_simulation_stopped(table))
			break ;
		now = get_time_in_ms();
		if (now >= wake_up)
			break ;
		usleep(200);
	}
}

/* sim_start_delay:
*	Busy-wait loop that delays thread start until a shared start_time.
*	Used to synchronize all philosopher threads and the death watcher
*	so their timestamps are comparable from the same reference point.
*/
void	sim_start_delay(time_t start_time)
{
	time_t	now;

	while (true)
	{
		now = get_time_in_ms();
		if (now >= start_time)
			break ;
		usleep(500);
	}
}
