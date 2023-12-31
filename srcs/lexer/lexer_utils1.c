/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: letnitan <letnitan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:44:25 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/29 21:56:28 by letnitan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Redige la nouvelle string
en supprimant le dollard
si la variable designee n'existait pas */
char	*ft_removedollard(char *cmd, int size, int index)
{
	char	*new;
	int		i;
	int		j;

	new = ft_calloc(sizeof(char), size);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (i == index)
		{
			i++;
			while (cmd[i] && !ft_isspace(cmd[i]) && !ft_issyntax(cmd[i])
				&& cmd[i] != '$' && cmd[i] != 34 && cmd[i] != 39
				&& cmd[i] != ':')
				i++;
		}
		if (!cmd[i])
			break ;
		new[j++] = cmd[i++];
	}
	new[j] = '\0';
	return (new);
}

/* Redige la nouvelle string en remplacant
le dollard par sa valeur reelle */
char	*ft_replacedollard(char *cmd, char *env, int size, int *index)
{
	char	*new;
	int		i;
	int		j;
	int		k;

	new = ft_calloc(sizeof(char), size);
	if (!new)
		return (NULL);
	i = 0;
	k = 0;
	while (cmd[i] && i < *index)
		new[k++] = cmd[i++];
	j = 0;
	while (env[j])
		new[k++] = env[j++];
	*index = k;
	i++;
	while (cmd[i] && !ft_isspace(cmd[i]) && !ft_issyntax(cmd[i])
		&& cmd[i] != 34 && cmd[i] != 39 && cmd[i] != '$' && cmd[i] != ':')
		i++;
	while (cmd[i])
		new[k++] = cmd[i++];
	new[k] = '\0';
	return (new);
}

char	*ft_get_status(int status)
{
	char	*str;
	int		len;

	len = ft_nbrlen(status);
	str = ft_calloc(sizeof(char), (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (status == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (status < 0)
	{
		str[0] = '-';
		status *= -1;
	}
	while (status)
	{
		str[len--] = (status % 10) + '0';
		status /= 10;
	}
	return (str);
}

/* calcule la taille de la nouvelle commande,
et la fait rediger par la fonction appropriee */
char	*ft_write_new(t_shell *shell, char *cmd, char *env, int *index)
{
	int		i;
	int		size;

	size = 0;
	i = 0;
	while (cmd[i])
	{
		if (i == *index)
		{
			i++;
			while (cmd[i] && !ft_issyntax(cmd[i]) && !ft_isspace(cmd[i])
				&& cmd[i] != 34 && cmd[i] != 39 && cmd[i] != '$'
				&& cmd[i] != ':')
				i++;
		}
		if (!cmd[i])
			break ;
		size++;
		i++;
	}
	if (!env)
		return (ft_removedollard(cmd, size + 1, *index));
	size += ft_strlen(env);
	ft_add_to_the_bin(env, STR, shell->bin);
	return (ft_replacedollard(cmd, env, size + 1, index));
}

/* transforme le dollard par sa valeur reelle,
Ou le supprime si celui-ci n'existe pas */
void	ft_rewrite_dollard(t_shell *shell, t_elem *cur, int index, int *jindex)
{
	char	*new;
	char	*env;

	if (!ft_continue(cur->av[index][*jindex + 1], jindex))
		return ;
	if (cur->av[index][*jindex + 1] == '?')
	{
		env = ft_get_status(shell->error_status);
		new = ft_write_new(shell, cur->av[index], env, jindex);
	}
	else
	{
		env = ft_returnenv(cur->av[index], *jindex);
		new = ft_write_new(shell, cur->av[index],
				ft_getenv(shell->env, env), jindex);
	}
	free(cur->av[index]);
	cur->av[index] = new;
}
