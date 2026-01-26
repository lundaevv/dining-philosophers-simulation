/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_clock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 10:22:43 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:06:06 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* get_time_in_ms:
*	Returns current timestamp in milliseconds using gettimeofday().
*/
time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
