/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaqvi <anaqvi@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:55:21 by anaqvi            #+#    #+#             */
/*   Updated: 2025/02/13 18:35:55 by anaqvi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef volatile sig_atomic_t	t_signal;
typedef struct s_cmd_grp		t_cmd_grp;

extern t_signal					g_signal_received;

# define CTRL_OP_ERR "control operator not supported by minishell."
# define LOCAL_VAR_ERR "local variables are not supported by minishell."
# define MARK_EXP_ERR "marking for export is not possible since \
minishell does not support local variables."

/* Holds everything that needs to be cleaned up before exiting.
@param allocs Pointer to the start of linked list that contains
all malloced pointers that need to be freed.
@param open_fds Pointer to the start of linked list that contains
all open file descriptors that need to be closed.
*/
typedef struct s_garbage
{
	t_list		*allocs;
	t_list		*open_fds;
}	t_garbage;

/* Indicates whether a command is supposed to be executed
with execve or builtin functions defined by us. */
typedef enum e_cmd_type
{
	BUILTIN,
	EXTERNAL,
}	t_cmd_type;

/*Indicates the mode for signal handling*/
typedef enum e_mode
{
	INTERACTIVE,
	HEREDOC,
	CHILD,
	NON_INTERACTIVE,
}	t_mode;

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
struct			s_cmd_grp
{
	char		*cmd_name;
	char		**cmd_args;
	t_cmd_type	cmd_type;
	int			in_fd;
	int			out_fd;
	t_cmd_grp	*previous;
	t_cmd_grp	*next;
};

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

// asdasd< "something in put" 123
// "asdasd", "<", "something in put", "123"

// parser
/*- Take the `char **` from lexer and form "groups" of commands,
separated by pipes (`|`), and save them as a linked list of t_cmd_grp.
- Create pipes with `pipe` function
- If command as given is not executable, search the `PATH` to look for
the command and make it `execve`-ready*/
int				parser(t_minishell *minishell);
/*`s1` and `s2` are expected to be redirection operator and filename
(or delimiter in case of heredoc), respectively Depending on the operator,
open or create the file, or prompt the user for input in case of heredoc.
Update the `cmd_grp_node.in_fd` and `cmd_grp_node.out_fd` accordingly.
Return 0 on success and -1 on error, printing error message accordingly.*/
int				update_cmd_grp_fds(t_minishell *minishell,
					t_cmd_grp *cmd_grp_node, char *s1, char *s2);
/*Update `cmd_grp_node.cmd_name` and `cmd_grp_node.cmd_args` depending on
`minishell.tokenized[*i]` until a `|`, NULL, or an unsupported token
is encountered, incrementing `*i` accordingly. Also update
`cmd_grp_node.cmd_type`. If `minishell.tokenized[*(i-1)]` is a pipe, create pipe
and update `cmd_grp_node.in_fd` and `cmd_grp_node.out_fd` of the appropriate
cmd_grp. Return 0 on success and -1 on error, printing error message
accordingly.*/
int				update_cmd_grp_cmds(t_minishell *minishell, int *i,
					t_cmd_grp *cmd_grp_node);
/*Replace `cmd_name` in `cmd_grp_node` with its absolute path, if found on
the `PATH` and if `cmd_name` is not a builtin */
int				find_full_cmd_path(t_minishell *minishell,
					t_cmd_grp *cmd_grp_node);
/*Return 1 if `token` is one of the four redirection operators, 0 otherwise*/
int				is_redir_opr(char *token);
/*Create a child process that reads from heredoc until a delimiter is
encountered, an EOF or `SIGINT` is received, or an error has occured*/
int				handle_heredoc(char *delimiter, t_minishell *minishell);

// builtins
/* Similar to bash's echo but only takes the option -n*/
void			ft_echo(char **args, t_minishell *minishell);
/*Similar to cd builtin of bash, except it only supports a relative
or absolute path and does not support flags/options.*/
void			ft_cd_parent(char **args, t_minishell *minishell);
/*Similar to cd builtin of bash, except it only supports a relative
or absolute path and does not support flags/options. Exit process after
execution.*/
void			ft_cd_child(char **args, t_minishell *minishell);
/*Use `getcwd` to print the absolute pathname of the current working
directory.*/
void			ft_pwd(t_minishell *minishell);
/*Similar to export builtin of bash, except it does not support flags/options
or marking local variables for export.*/
void			ft_export_parent(char **args, t_minishell *minishell);
/*Similar to export builtin of bash, except it does not support flags/options
or marking local variables for export. Exit process after execution.*/
void			ft_export_child(char **args, t_minishell *minishell);
/*Similar to unset builtin of bash, except it does not support flags/options.
Exit process after execution.*/
void			ft_unset_child(char **args, t_minishell *minishell);
/*Similar to unset builtin of bash, except it does not support flags/options.*/
void			ft_unset_parent(char **args, t_minishell *minishell);
/*Search the evironmental variables to look for a variable named `var_name`.
Return the value of the variable if it is found. Otherwise return `NULL`.
The returned `char *` can be freed with `gc_free`.*/
char			*get_env_var_value(char *var_name, t_minishell *minishell);
/*The environmental variable named `var_name` is updated or added
(depending on whether it already exists) with value `var_val`.*/
void			update_env_var(char *var_name, char *var_val,
					t_minishell *minishell);
/*Similar to env builtin of bash, except it does not support flags/options
or arguments.*/
void			ft_env(t_minishell *minishell);
/*Similar to exit builtin of bash, except it does not support flags/options.*/
void			ft_exit_parent(char **args, t_minishell *minishell);
/*Similar to exit builtin of bash, except it does not support flags/options.
Exits process in all cases (including error).*/
void			ft_exit_child(char **args, t_minishell *minishell);

// Execution
/*Execute the pipeline in the linked list of `minishell.cmd_grp_strt`
Update `minishell.last_exit_status` based on the exit status of the last
command in the pipeline*/
void			execution(t_minishell *minishell);
/*Traverse the command group list (`minishell.cmd_grp_strt`) and execute
the command group indicated by the index `i`. After execution, exit the child
with `exit` and the appropriate exit status*/
void			execute_ith_cmd_grp(int i, t_minishell *minishell);

// Garbage collector:
/*Allocate `size` bytes of memory and add it to the garbage collector.
If malloc fails, exit the program immediately.*/
void			*gc_malloc(size_t size, t_minishell *minishell);
/*
	Free the old memory and allocate new memory of the size
	The new memory will still contain all the stuff that the old memory did
*/
void			*gc_realloc(size_t size, void *old_alloc,
					t_minishell *minishell);
/* Add `ptr` to the garbage collector*/
void			gc_add_to_allocs(void *ptr, t_minishell *minishell);
/* Find the node that contains `ptr`, free `ptr`, and delete the node
from the list (and free it)*/
void			gc_free(void *ptr, t_minishell *minishell);
/*Custom `open` that adds the fd to the garbage collector.
If opening fails, exits the program immediately. */
int				gc_open(char *file, int flags, mode_t mode,
					t_minishell *minishell);
/*Add `fd` to garbage collector. Should be used in case a file descriptor
was opened through some means other than `gc_open*/
void			gc_add_to_open_fds(int fd, t_minishell *minishell);
/*Find the node that contains `fd`, close `fd`, and delete the node
from the list (and free it)*/
void			gc_close(int fd, t_minishell *minishell);
/*Free/close everything in the garbage collector and exit with
the provided exit code*/
void			gc_exit(t_minishell *minishell, int exit_status);
/*Only meant to be used in the garbage collector. Do not use elsewhere*/
void			free_check_null(void *ptr);
/*Only meant to be used in the garbage collector. Do not use elsewhere*/
void			ft_close(void *ptr);
/*Close all fds in the garbage collector*/
void			gc_close_all_open_fds(t_minishell *minishell);
/*Use gc_free and gc_close to clean up the linked list of `t_cmd_grp`*/
void			gc_free_cmd_grps(t_minishell *minishell);
/*Free the `char **` given to it using gc_free*/
void			gc_free_2d_char_arr(char **arr, t_minishell *minishell);

// signals
/*Depending on the mode given, set up the appropriate signal handling function
that will be executed when `SIGINT` or `SIGQUIT` is received*/
int				set_signal_handler(t_mode mode);

// utils
/*Get the size of a NULL-terminated array of strings
(NULL is not counted)*/
int				get_array_size(char **arr);
/*Make a copy of a `char **` (2-dimensional array of characters).
If malloc fails, exit the program immediately.*/
char			**copy_2d_char_arr(char **arr, t_minishell *minishell);
void			ft_readline(t_minishell *minishell);

// VECTOR STUFF
// The idea of this type is that you can just use convenient functions on it
// Without having to worry about any size checking
// Everything will be automatic
typedef struct s_vec
{
	size_t		element_size;
	size_t		length;
	void		*elements; // I THINK this is how it should be
}	t_vec;

t_vec			vec_init(size_t element_size);
void			vec_increase_size(t_minishell *minishell, t_vec *vec);
void			vec_push_copy(t_minishell *minishell, t_vec *vec,
					void *element);
void			vec_push_ref(t_minishell *minishell, t_vec *vec, void *element);
void			*vec_get(t_vec *vec, size_t index);
void			vec_print_as_strings(t_vec *vec);
void			vec_print_as_tokens(t_vec *vec);

// TOKENIZER STUFF
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR,
	TOKEN_SPACE,
}	t_tok_type;

typedef struct s_token
{
	char		*string;
	size_t		read_length;
	size_t		parsed_length;
	t_tok_type	type;
}	t_token;

typedef struct s_count_with_quote
{
	size_t		count;
	int			ends_with_quote;
}	t_count_with_quote;

int				tokenizer(t_minishell *minishell);
t_token			get_next_token(t_minishell *minishell, size_t index);
t_token			tokenize_space(t_minishell *minishell, size_t index);
t_token			tokenize_quote(t_minishell *minishell, size_t index,
					char quote);
t_token			tokenize_word(t_minishell *minishell, size_t index);
t_token			tokenize_operator(t_minishell *minishell, size_t index);
void			tokens_to_array(t_minishell *minishell, t_vec *vec);

char			*replace_environment_variables(t_minishell *minishell,
					char *input);

// TOKENIZER UTILS
// Return 1 in case the character is a space character
// ...defined in the `man isspace`
int				ft_isspace(char c);
int				ft_isquote(char c);
int				ft_isoperator(char c);

// Tokenizer debug things
void			print_tokenized(char **tokenized);

// Environment Variables
typedef struct s_replaced_variable
{
	char		*value;
	size_t		key_len;
}	t_replaced_var;

char			*replace_env(t_minishell *minishell, char *word);
char			*get_env(t_minishell *minishell, char *key);
char			*cut_front(t_minishell *minishell, char *str, size_t cut_len);
size_t			get_env_key_len(char *env);
int				is_env_char(char c);
size_t			count_key_len(char *str);
t_replaced_var	cut_variable(t_minishell *minishell, char *str);
char			*str_add_char(t_minishell *minishell, char *str, char c);
/*Write an error to `stderr` in the following format:
"minishell: `problem`: `msg`"*/
void			put_specific_error(char *problem, char *msg);
/*Write an error to `stderr` in one of the following formats:
- if `problem` is an empty string: "minishell: `builtin_name`: `msg`"
- otherwise: "minishell: `builtin_name`: `problem`: `msg`"*/
void			put_builtin_error(char *builtin_name, char *problem, char *msg);
/*Like `perror` but prepends "minishell: " before `perrors`'s output*/
void			shell_error(const char *msg);
/*Extract exit status using bitwise operations on `status` set by `waitpid`.
If the exit status could not be extracted, 1 is returned.*/
int				get_exit_status(int status);
/*Return a substring of `str` until the first occurance of `c` (not including
`c`). `c` is assumed to be present in `str`. Result can be freed with `gc_free`.
Please do not use `free` on the result.*/
char			*substr_before_char(char *str, char c, t_minishell *minishell);
/*Return a substring of `str` starting from the character after the first
occurance of `c` until the end of `str`. `c` is assumed to be present in `str`.
Result can be freed with `gc_free`. Please do not use `free` on the result.*/
char			*substr_after_char(char *str, char c, t_minishell *minishell);
/*ft_substr's memory-safe alternative that utilizes the garbage collector.
Checks for `malloc` failure. Result can be freed with `gc_free`. Please do not
use `free` on the result.*/
char			*gc_ft_substr(char *s, unsigned int start, size_t len,
					t_minishell *minishell);
/*ft_strjoin's memory-safe alternative that utilizes the garbage collector.
Checks for `malloc` failure. Result can be freed with `gc_free`. Please do not
use `free` on the result.*/
char			*gc_ft_strjoin(char *s1, char *s2, t_minishell *minishell);
/*ft_strdup's memory-safe alternative that utilizes the garbage collector.
Checks for `malloc` failure. Result can be freed with `gc_free`. Please do not
use `free` on the result.*/
char			*gc_ft_strdup(char *s1, t_minishell *minishell);
/*Convert `str` into its integer equivalent.
Return 0 on success and -1 on error.*/
int				ft_atoi_error(char *str, int *num);

#endif
