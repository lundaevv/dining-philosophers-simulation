/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:39:19 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:05:09 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* print_status:
*	Prints one simulation status line in required project format.
*	The timestamp is relative to the simulation start_time.
*/
static void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

/* write_status:
*	Thread-safe status printer.
*	write_lock ensures output lines never overlap.
*
*	If sim_stop is already true:
*	- philosophers stop printing actions
*	- death_watcher is allowed to print the death line once
*/
void	write_status(t_philo *philo, bool watcher, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) == true && watcher == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}
