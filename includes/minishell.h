/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:55:21 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/07 15:18:02 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>

/* Holds everything that needs to be cleaned up before exiting.
@param allocs Pointer to the start of linked list that contains
all malloced pointers that need to be freed.
@param open_fds Pointer to the start of linked list that contains
all open file descriptors that need to be closed.
*/
typedef struct s_garbage
{
	t_list	*allocs;
	t_list	*open_fds;
}	t_garbage;

/* Indicates whether a command is supposed to be executed
with execve or builtin functions defined by us. */
typedef enum e_cmd_type
{
	BUILTIN,
	EXTERNAL,
}	t_cmd_type;

/* Holds a command group. One command group contains the command-line
information delimited by pipes or by start/end of the command-line.
*
@param cmd_name Full name with path e.g., /usr/bin/ls.
@param cmd_args e.g., {"ls", "-al", NULL}.
@param cmd_type Enum with two possible values: BUILTIN or EXTERNAL.
@param in_fd Default is stdin, but could be the write end of a pipe.
(if | comes before the cmd group) or a file redirection
(if < or << is present in the cmd group).
@param out_fd Default is stdout, but could be the read end of a pipe
(if | comes after the cmd group) or a file redirection
(if > oor >> is present in the cmd group).
@param previous Previous node in the linked list.
@param next Next node in the linked list.
*/
typedef struct s_cmd_grp
{
	char				*cmd_name;
	char				**cmd_args;
	t_cmd_type			cmd_type;
	int					in_fd;
	int					out_fd;
	struct s_cmd_grp	*previous;
	struct s_cmd_grp	*next;
}	t_cmd_grp;

/* The main struct to be passed to most functions. Contains all the
important variables of the minishell program.
@param cmd_grp_strt Start of the command group linked list.
@param garbage Garbage collector struct.
@param input The line that user typed on the command line
received with readline function.
@param mini_env Copy of envp. Represents the environment of minishell.
*/
typedef struct s_minishell
{
	t_cmd_grp	*cmd_grp_strt;
	t_garbage	garbage;
	char		*input;
	char		**mini_env;
	char		**tokenized;
}	t_minishell;

// parser
/*- Take the `char **` from lexer and form “groups” of commands,
separated by pipes (`|`), and save them as a linked list of t_cmd_grp.
- Create pipes with `pipe` function
- If command as given is not executable, search the `PATH` to look for
the command and make it `execve`-ready*/
int	parser(t_minishell *minishell);

// Garbage collector:
/*Allocate `size` bytes of memory and add it to the garbage collector.
If malloc fails, exit the program immediately.*/
void	*gc_malloc(size_t size, t_minishell *minishell);
/* Add `ptr` to the garbage collector*/
void	gc_add_to_allocs(void *ptr, t_minishell *minishell);
/* Find the node that contains `ptr`, free `ptr`, and delete the node
from the list (and free it)*/
void	gc_free(void *ptr, t_minishell *minishell);
/*Custom `open` that adds the fd to the garbage collector.
If opening fails, exits the program immediately. */
int		gc_open(char *file, int flags, mode_t mode, t_minishell *minishell);
/*Add `fd` to garbage collector. Should be used in case a file descriptor 
was opened through some means other than `gc_open*/
void	gc_add_to_open_fds(int fd, t_minishell *minishell);
/*Find the node that contains `fd`, close `fd`, and delete the node
from the list (and free it)*/
void	gc_close(int fd, t_minishell *minishell);
/*Free/close everything in the garbage collector and exit with
the provided exit code*/
void	gc_exit(t_minishell *minishell, int exit_status);
/*Only meant to be used in the garbage collector. Do not use elsewhere*/
void	free_check_null(void *ptr);

// utils
/*Make a copy of a `char **` (2-dimensional array of characters).
If malloc fails, exit the program immediately.*/
char	**copy_2d_char_arr(char **arr, t_minishell *minishell);

#endif
