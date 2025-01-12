/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:55:21 by anaqvi            #+#    #+#             */
/*   Updated: 2025/01/11 19:18:23 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>

extern volatile sig_atomic_t	g_signal_received;

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
(if > or >> is present in the cmd group).
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
	int			last_exit_status;
}	t_minishell;

// parser
/*- Take the `char **` from lexer and form “groups” of commands,
separated by pipes (`|`), and save them as a linked list of t_cmd_grp.
- Create pipes with `pipe` function
- If command as given is not executable, search the `PATH` to look for
the command and make it `execve`-ready*/
int		parser(t_minishell *minishell);
/*`s1` and `s2` are expected to be redirection operator and filename
(or delimiter in case of heredoc), respectively Depending on the operator,
open or create the file, or prompt the user for input in case of heredoc.
Update the `cmd_grp_node.in_fd` and `cmd_grp_node.out_fd` accordingly.
Return 0 on success and -1 on error, printing error message accordingly.*/
int		update_cmd_grp_fds(t_minishell *minishell, t_cmd_grp *cmd_grp_node,
			char *s1, char *s2);
/*Update `cmd_grp_node.cmd_name` and `cmd_grp_node.cmd_args` depending on
`minishell.tokenized[*i]` until a `|`, NULL, or an unsupported token
is encountered, incrementing `*i` accordingly. Also update
`cmd_grp_node.cmd_type`. If `minishell.tokenized[*(i-1)]` is a pipe, create pipe
and update `cmd_grp_node.in_fd` and `cmd_grp_node.out_fd` of the appropriate
cmd_grp. Return 0 on success and -1 on error, printing error message
accordingly.*/
int		update_cmd_grp_cmds(t_minishell *minishell, int *i,
			t_cmd_grp *cmd_grp_node);
/*Replace `cmd_name` in `cmd_grp_node` with its absolute path, if found on
the `PATH` and if `cmd_name` is not a builtin */
int		find_full_cmd_path(t_minishell *minishell, t_cmd_grp *cmd_grp_node);
/*Return 1 if `token` is one of the four redirection operators, 0 otherwise*/
int		is_redir_opr(char *token);

// Execution
/*Executes the pipeline in the linked list of `minishell.cmd_grp_strt`
Updates `minishell.last_exit_status` based on the exit status of the last
command in the pipeline*/
void	execution(t_minishell *minishell);

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
/*Use gc_free and gc_close to clean up the linked list of `t_cmd_grp`*/
void	gc_free_cmd_grps(t_minishell *minishell);
/*Free the `char **` given to it using gc_free*/
void	gc_free_2d_char_arr(char **arr, t_minishell *minishell);

// utils
/*Make a copy of a `char **` (2-dimensional array of characters).
If malloc fails, exit the program immediately.*/
char	**copy_2d_char_arr(char **arr, t_minishell *minishell);

#endif
