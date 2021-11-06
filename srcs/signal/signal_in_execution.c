/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_in_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnakahod <jnakahod@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 18:14:21 by jnakahod          #+#    #+#             */
/*   Updated: 2021/11/06 14:22:10 by jnakahod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mini_shell.h>

extern t_request	g_request;

void	interrupt_in_execution(int sig_id)
{
	g_request.exit_cd = sig_id + 128;
	rl_replace_line("", 0);
	ft_putstr_fd("\n", 1);
}

void	int_act_in_execution(void)
{
	struct sigaction	int_act;

	ft_bzero(&int_act, sizeof(int_act));
	int_act.sa_handler = interrupt_in_execution;
	if (sigaction(SIGINT, &int_act, NULL) != 0)
		exit(FAILURE);
}

void	quit_in_execution(int sig_id)
{
	g_request.exit_cd = sig_id + 128;
	printf("Quit: %d\n", sig_id);
}

void	quit_act_in_execution(void)
{
	struct sigaction	quit_act;

	ft_bzero(&quit_act, sizeof(quit_act));
	quit_act.sa_handler = quit_in_execution;
	if (sigaction(SIGQUIT, &quit_act, NULL) != 0)
		exit(FAILURE);
}

void	init_signal_in_execution(void)
{
	int_act_in_execution();
	quit_act_in_execution();
}
