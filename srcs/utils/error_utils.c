/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:16:29 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/29 23:46:20 by hedubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_errornext(t_shell *shell, t_error error, char *arg)
{
	if (error == ARG)
	{
		shell->error_status = 1;
		ft_putstr_fd("Minishell : too many arguments", 2);
		exit(EXIT_FAILURE);
	}
	else if (error == NOPATH)
	{
		shell->error_status = 127;
		ft_putstr_fd("MiniAlfred: ", 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

bool	ft_error(char *arg, t_error error, t_shell *shell)
{
	extern int	g_error;

	if (error == SYN)
	{
		shell->error_status = 2;
		ft_putstr_fd("MiniAlfred: syntax error near unexpected token\n", 2);
	}
	else if (error == CMD)
	{
		shell->error_status = 126;
		ft_putstr_fd("MiniAlfred: ", 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	ft_errornext(shell, error, arg);
	return (false);
}
