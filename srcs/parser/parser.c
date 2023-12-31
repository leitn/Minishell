/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:03:06 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/29 22:53:20 by hedubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* identifie si un char est un
element de syntaxe devant declencher
un comportement particulier */

t_syntax	ft_issyntax(char c)
{
	if (c == '<' || c == '>')
		return (REDIR);
	if (c == 124)
		return (PIPE);
	if (c == '-')
		return (OPTION);
	return (NO);
}

t_syntax	ft_istoken(char c)
{
	static int	tem = 0;

	if (ft_issyntax(c))
	{
		tem = 1;
		return (ft_issyntax(c));
	}
	if (ft_isspace(c) && tem == 0)
		return (WSPACE);
	tem = 0;
	return (NO);
}

/* Parse l'input, et redige l'arbre
envoye au lexer puis a l'execution
si aucune erreur n'est rencontree.*/

bool	ft_parse(t_shell *shell)
{
	int			i;

	if (!shell->input)
		return (false);
	if (ft_isempty(shell->input))
		return (ft_filter(shell, INPONLY), false);
	if (ft_unclosedquotes(shell->input) || ft_error_pipe_or_redir(shell->input))
		return (ft_filter(shell, INPONLY), ft_error(NULL, SYN, shell));
	if (!ft_init_tree(shell))
		return (ft_filter(shell, INPONLY), false);
	i = 0;
	while (shell->input[i])
	{
		if (!ft_write_elem(shell, &i))
			return (ft_filter(shell, BOTH), false);
	}
	ft_lex(shell);
	ft_manage_paths(shell);
	if (!ft_manage_fds(shell))
		return (ft_filter(shell, BOTH), false);
	return (ft_filter(shell, INPONLY), true);
}

/* suite de ft_size dans parse_utils3.c (norme) */
int	ft_end_size(char *input, int *end, int size, int token)
{
	if (((!input[*end] && input[*end - 1] != ' ') || (input[*end] == '|'
				&& !ft_isspace(input[*end - 1]))) && token != REDIR)
		size++;
	while (input[*end] && ft_isspace(input[*end - 1]))
		*end -= 1;
	return (size);
}

void	ft_manage_paths(t_shell *shell)
{
	t_elem	*tmp;

	shell->tree->ispath = true;
	tmp = shell->tree->first;
	while (tmp)
	{
		if (tmp->av)
			ft_write_path(shell, tmp);
		else
			tmp->path = NULL;
		tmp = tmp->next;
	}
	if (!shell->env->paths)
		shell->tree->ispath = false;
}
