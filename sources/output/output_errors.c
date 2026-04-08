/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 18:17:57 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/26 22:04:58 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* print_msg:
*	Prints a formatted message to stdout.
*	If detail is NULL, prints only the base string with program name.
*	Returns exit_no for convenient "return (print_msg(...))" usage.
*/
int	print_msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/* error_return_null:
*	Handles an error that should return NULL.
*	Cleans up table if it exists, prints error message and returns NULL.
*/
void	*error_return_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		cleanup_table(table);
	print_msg(str, details, EXIT_FAILURE);
	return (NULL);
}
