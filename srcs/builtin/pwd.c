/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsugiyam <tsugiyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 23:12:29 by tsugiyam          #+#    #+#             */
/*   Updated: 2021/05/29 23:12:29 by tsugiyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mini_shell.h>

t_bool execute_pwd(const char **cmd_args)
{
  char pwd[BUFSIZ];

  (void)cmd_args;
  pwd[0] = '0';
  getcwd(pwd, BUFSIZ);
  ft_putendl_fd(pwd, STDOUT);
  return (TRUE);
}
