/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:25:30 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/09 15:21:29 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_paths(char **mini_env, t_minishell *minishell)
{
	char	**paths;
	int		i;

	if (!mini_env)
		return (NULL);
	while (*mini_env)
	{
		if (!ft_strncmp("PATH=", *mini_env, 5))
			break ;
		mini_env++;
	}
	if (!(*mini_env))
		return (NULL);
	paths = ft_split((*mini_env) + 5, ':');
	if (!paths)
		return (gc_exit(minishell, EXIT_FAILURE), NULL);
	gc_add_to_allocs(paths, minishell);
	i = 0;
	while (paths[i])
	{
		gc_add_to_allocs(paths[i], minishell);
		i++;
	}
	return (paths);
}

static void	find_executable(t_cmd_grp *cmd_grp_node, char **paths,
t_minishell *minishell)
{
	char	*current_path;
	size_t	len;

	while (*paths)
	{
		len = sizeof(char) * (ft_strlen(*paths)
				+ ft_strlen(cmd_grp_node->cmd_name) + 2);
		current_path = gc_malloc(len, minishell);
		ft_strlcpy(current_path, *paths, len);
		ft_strlcat(current_path, "/", len);
		ft_strlcat(current_path, cmd_grp_node->cmd_name, len);
		if (access(current_path, X_OK) == 0)
			break ;
		gc_free(current_path, minishell);
		current_path = NULL;
		paths++;
	}
	if (!current_path)
		return ;
	gc_free(cmd_grp_node->cmd_name, minishell);
	cmd_grp_node->cmd_name = current_path;
	return ;
}

int	find_full_cmd_path(t_minishell *minishell, t_cmd_grp *cmd_grp_node)
{
	char	**paths;

	if (cmd_grp_node->cmd_type == EXTERNAL)
	{
		if (!access(cmd_grp_node->cmd_name, X_OK))
			return (0);
		paths = get_paths(minishell->mini_env, minishell);
		find_executable(cmd_grp_node, paths, minishell);
		gc_free_2d_char_arr(paths, minishell);
	}
	return (0);
}
