/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 16:39:19 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 23:19:44 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* print_status:
*	Prints one simulation status line in required project format.
*	The timestamp is relative to the simulation start_time.
*/
static void	print_status(t_philo *philo, char *str)
{
	time_t	now;
	time_t	ts;

	now = get_time_in_ms();
	ts = now - philo->table->start_time;
	if (ts < 0)
		ts = 0;
	printf("%ld %d %s\n", ts, philo->id + 1, str);
}

/* write_status:
*	Thread-safe status printer.
*	write_lock ensures output lines never overlap.
*
*	Printing rules:
*	- If watcher == false (philosopher thread) and sim_stop is already true,
*	  do not print anything.
*	- If watcher == true (death watcher / special caller), printing is allowed
*	  even after sim_stop (used for the final "died" message).
*
*	We check sim_stop twice:
*	1) before locking write_lock (fast path)
*	2) after locking write_lock (prevents rare race where sim_stop becomes true
*	   between the first check and the lock acquisition).
*/
void	write_status(t_philo *philo, bool watcher, t_status status)
{
	if (!watcher && has_simulation_stopped(philo->table))
		return ;
	pthread_mutex_lock(&philo->table->write_lock);
	if (!watcher && has_simulation_stopped(philo->table))
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
	else
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}
