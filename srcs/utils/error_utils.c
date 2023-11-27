/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letnitan <letnitan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:16:29 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/27 23:13:40 by letnitan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ft_error(char *arg, t_error error)
{
	extern int	g_error;

	if (error == SYN)
	{
		g_error = 2;
		ft_putstr_fd("MiniAlfred: syntax error near unexpected token\n", 2);
	}
	else if (error == CMD)
	{
		g_error = 126;
		ft_putstr_fd("MiniAlfred: ", 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (error == ARG)
	{
		ft_putstr_fd("Minishell : too many arguments", 2);
		exit(EXIT_FAILURE);
	}
	return (false);
}
