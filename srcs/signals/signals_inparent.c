/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 15:04:05 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/24 11:39:31 by hedubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Ignore ctrl-z et ctrl-/,
redirige ctrl-c. ctrl-d est gere
par readline directement. */

t_shell	*ft_getshell(t_shell *shell, int trigger)
{
	static t_shell	*saved_shell;

	if (trigger == 1)
	{
		saved_shell = shell;
		return (NULL);
	}
	return (saved_shell);
}

void	ft_signals_inparent(void)
{
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &ft_ctrlc_inparent);
}

void	ft_ctrlc_inparent(int sig)
{
	extern int	g_error;
	static char	*red = " \033[31mMiniAlfred\033[0m ";

	(void)sig;
	g_error = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_putchar_fd('\n', 1);
	ft_putstr_fd(red, 1);
	rl_redisplay();
}