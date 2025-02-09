/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:45:31 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/09 20:59:01 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_strerror(char *cd_path)
{
	char	*err_str;

	if (errno == ENOENT)
		err_str = "No such file or directory";
	else if (errno == EACCES)
		err_str = "Permission denied";
	else if (errno == ENOTDIR)
		err_str = "Not a directory";
	else
		err_str = strerror(errno);
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(cd_path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(err_str, STDERR_FILENO);
}

static char	*get_home_dir(t_minishell *minishell)
{
	char	*home_dir;

	home_dir = get_var_value("HOME", minishell);
	if (!home_dir)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		gc_exit(minishell, EXIT_FAILURE);
		return (NULL);
	}
	return (home_dir);
}

void	ft_cd_child(char **args, t_minishell *minishell)
{
	int		argc;
	char	*cd_path;
	char	*old_pwd;

	if (!args || !(*args))
		return(gc_exit(minishell, EXIT_FAILURE));
	argc = get_array_size(args);
	if (argc > 2)
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO),
			gc_exit(minishell, EXIT_FAILURE));
	if (argc == 1)
		cd_path = get_home_dir(minishell);
	else
		cd_path = args[1];
	if (chdir(cd_path) == -1)
	{
		ft_strerror(cd_path);
		return (gc_exit(minishell, EXIT_FAILURE));
	}
	old_pwd = get_var_value("PWD", minishell);
	if (old_pwd)
		update_env_var("OLDPWD", old_pwd, minishell);
	update_env_var("PWD", cd_path, minishell);
	gc_exit(minishell, EXIT_SUCCESS);
}

void	ft_cd_parent(char **args, t_minishell *minishell)
{
	int		argc;
	char	*cd_path;
	char	*old_pwd;

	if (!args || !(*args))
		return(gc_exit(minishell, EXIT_FAILURE));
	argc = get_array_size(args);
	if (argc > 2)
	{
		minishell->last_exit_status = EXIT_FAILURE;
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO));
	}
	if (argc == 1)
		cd_path = get_home_dir(minishell);
	else
		cd_path = args[1];
	if (chdir(cd_path) == -1)
		return (ft_strerror(cd_path));
	old_pwd = get_var_value("PWD", minishell);
	if (old_pwd)
		update_env_var("OLDPWD", old_pwd, minishell);
	update_env_var("PWD", cd_path, minishell);
}
