/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:56:59 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/29 23:42:32 by hedubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ft_freestr(char *str)
{
	free(str);
	str = NULL;
	return (false);
}

void	ft_free_redirs(t_red *redirs)
{
	t_red	*tmp1;
	t_red	*tmp2;

	tmp1 = redirs;
	while (tmp1)
	{
		tmp2 = tmp1->next;
		free(tmp1->av);
		free(tmp1);
		tmp1 = tmp2;
	}
}

void	ft_free_tmpfile(char *hd_name, int fd)
{
	close(fd);
	unlink(hd_name);
	free(hd_name);
}

void	ft_free_tree(t_head *tree)
{
	t_elem	*tmp1;
	t_elem	*tmp2;

	tmp1 = tree->first;
	while (tmp1)
	{
		tmp2 = tmp1->next;
		if (tmp1->av)
			ft_free_str_array(tmp1->av);
		if (tmp1->hd_name != NULL)
			ft_free_tmpfile(tmp1->hd_name, tmp1->fd_rd);
		if (tmp1->path)
			free(tmp1->path);
		if (tmp1->redirs)
			ft_free_redirs(tmp1->redirs);
		free(tmp1);
		tmp1 = NULL;
		tmp1 = tmp2;
	}
	free(tree);
}

void	ft_free_str_array(char **strarr)
{
	char	**tmp;

	tmp = strarr;
	while (*tmp)
	{
		free(*tmp);
		*tmp = NULL;
		tmp++;
	}
	free(strarr);
	strarr = NULL;
}
