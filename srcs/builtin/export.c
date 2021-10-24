/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsugiyam <tsugiyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 15:25:38 by tsugiyam          #+#    #+#             */
/*   Updated: 2021/10/22 21:49:24 by tsugiyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mini_shell.h>

extern t_request	g_request;

t_bool	is_key_exist(char *key)
{
	t_environ	*environ;

	environ = g_request.environs;
	while (environ)
	{
		if (!ft_strcmp(environ->key, key))
			return (TRUE);
		environ = environ->next;
	}
	return (FALSE);
}

t_bool	replace_duplicated_environ(char *key, char *value)
{
	t_environ	*environ;

	environ = g_request.environs;
	while (environ)
	{
		if (!ft_strcmp(environ->key, key))
		{
			free(environ->value);
			environ->value = ft_strdup(value);
			return (TRUE);
		}
		environ = environ->next;
	}
	return (FALSE);
}

char	**split_key_value(char *arg)
{
	int		i;
	char	**split;

	split = malloc(sizeof(char *) * 3);
	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	split[0] = ft_strndup(arg, i);
	if (ft_strlen(arg) == ft_strlen(split[0]))
	{
		free(split[0]);
		free(split);
		return (NULL);
	}
	split[1] = ft_strdup(arg + i + 1);
	split[2] = NULL;
	return (split);
}

t_exit_cd	declare_env(void)
{
	t_environ	*environ;

	environ = g_request.environs;
	while (environ)
	{
		ft_putstr_fd("declare -x ", STDOUT);
		ft_putstr_fd(environ->key, STDOUT);
		ft_putstr_fd("=", STDOUT);
		ft_putstr_fd(environ->value, STDOUT);
		ft_putstr_fd("\n", STDOUT);
		environ = environ->next;
	}
	return (SCCSS);
}

t_exit_cd	execute_export(const char **cmd_args, t_bool is_child_process)
{
	char	**split;
	int i;

	if (!cmd_args[1])
	{
		declare_env();
		return (return_or_exit(SCCSS, is_child_process));
	}
	i = 0;
	while (cmd_args[++i])
	{
		split = split_key_value((char *)cmd_args[i]);
		if (!split)
			return (builtin_err(NULL, GNRL_ERR, is_child_process));
		if (!replace_duplicated_environ(split[0], split[1]))
			append_environ(&g_request.environs,
				new_environ(ft_strdup(split[0]), ft_strdup(split[1])));
		multi_free(split);
	}
	return (return_or_exit(SCCSS, is_child_process));
}
