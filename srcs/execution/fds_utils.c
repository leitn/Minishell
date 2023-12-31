/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 22:39:49 by letnitan          #+#    #+#             */
/*   Updated: 2023/11/29 23:21:23 by hedubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_hd(t_shell *shell)
{
	t_elem	*cur;

	cur = shell->tree->first;
	while (cur)
	{
		if (cur->hd_name != NULL)
		{
			unlink(cur->hd_name);
			free(cur->hd_name);
			cur->hd_name = NULL;
		}
		cur = cur->next;
	}
}

int	ft_close_fds(t_shell *shell, t_elem *cur)
{
	int		i;
	t_elem	*tmp;

	i = 0;
	tmp = cur;
	if (shell->tree->count_pipe > 0)
	{
		while (i < shell->tree->count_pipe)
		{
			close(shell->pipe[i][0]);
			close(shell->pipe[i][1]);
			i++;
		}
	}
	while (tmp)
	{
		if (tmp->fd_rd > 0)
			close(tmp->fd_rd);
		if (tmp->fd_wr > 1)
			close(tmp->fd_wr);
		tmp = tmp->next;
	}
	return (0);
}

int	dup_no_pipe(t_shell *shell, t_elem *cur, int i)
{
	(void)shell;
	i = 0;
	if (cur->fd_rd != 0 && cur->fd_rd > 0)
	{
		if (dup2(cur->fd_rd, STDIN_FILENO) == -1)
		{
			perror("Error Dup_no_pipe fd_rd");
			return (155);
		}
	}
	if (cur->fd_wr != 1 && cur->fd_wr > 1)
	{
		if (dup2(cur->fd_wr, STDOUT_FILENO) == -1)
		{
			perror("Error Dup_no_pipe fd_wr");
			return (155);
		}
	}
	return (0);
}

int	dup_pipe_rd(t_shell *shell, int i)
{
	if (shell->tree->ispath == true)
	{
		if (dup2(shell->pipe[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("Error Dup_pipe_rd");
			return (155);
		}
	}
	return (0);
}

int	dup_pipe_wr(t_shell *shell, int i)
{
	if (dup2(shell->pipe[i][1], STDOUT_FILENO) == -1)
	{
		perror("Error Dup2");
		return (155);
	}
	return (0);
}
