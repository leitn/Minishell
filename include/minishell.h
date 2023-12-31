/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedubois <hedubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:59:01 by hedubois          #+#    #+#             */
/*   Updated: 2023/11/29 23:57:08 by hedubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/***** INCLUDES *****/

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <stdbool.h>

/***** GLOBALE *****/

extern int	g_error;

/***** STRUCTURES *****/

typedef enum t_type
{
	STR,
	INTARR,
	STRARR,
}	t_type;

typedef struct s_waste
{
	void			*waste;
	t_type			type;
	struct s_waste	*next;
}	t_waste;

typedef struct s_bin
{
	struct s_waste	*first;
	bool			isempty;
}	t_bin;

typedef struct s_env
{
	char	**envp;
	char	**paths;
}	t_env;

typedef enum s_syntax
{
	NO = 0,
	PIPE,
	REDIR,
	OPTION,
	SIMPLEREDIRLEFT,
	DOUBLEREDIRLEFT,
	SIMPLEREDIRRIGHT,
	DOUBLEREDIRRIGHT,
	PIPE_RD,
	PIPE_WR,
	WSPACE,
}	t_syntax;

typedef struct s_red
{
	t_syntax			syn;
	char				*av;
	struct s_red		*next;
}	t_red;

typedef struct s_elem
{
	char			**av;
	char			*path;
	char			*hd_name;
	int				fd_rd;
	int				fd_wr;
	t_red			*redirs;
	int				nbr_heredocs;
	struct s_elem	*next;
	struct s_elem	*prev;
}	t_elem;

typedef struct s_head
{
	t_elem			*first;
	bool			ispath;
	bool			pipe;
	int				count_pipe;
}	t_head;

typedef struct s_shell
{
	t_env			*env;
	t_head			*tree;
	bool			isthere;
	char			*input;
	t_bin			*bin;
	int				pipe[1024][2];
	int				pids[1024];
	t_elem			*exec_current;
	int				error_status;
	int				hd_fd;
	int				pidecho;
	bool			ss;
}	t_shell;

typedef enum s_filter
{
	INPONLY,
	TREEONLY,
	BOTH,
	FCLEAN,
}	t_filter;

typedef enum s_error
{
	SYN,
	CMD,
	ARG,
	NOPATH,
}	t_error;

/***** FUNCTIONS *****/

/** INITIALISATION **/

/* Gestion des frees :
l'arbre, les branches et leurs contenus,
l'input, la structure shell et tout ce qu'elle
contient sont free regulierement par ft_filter().
Il ne faut add aucune de ses choses ou element a
la corbeille. tout le reste peut l'etre sans probleme.
la corbeille gere actuellement des arrays de char
(type STR), des arrays d'array de char
et des arrays d'int (type INTARR) */

/* init_utils */

bool		ft_init_tree(t_shell *shell);
char		*ft_init_current(char *cur_env);
t_shell		*ft_init_shell(char **env);

/* init_utils2 */

bool		ft_get_paths(t_env *en, t_bin *bin);
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_realloc(void *ptr, int new_size, int old_size);

/* init_shlvl */

bool		ft_isshlvl(char *cur_env);
char		*ft_write_shlvl(char *cur_env);

/* bin_utils */

bool		ft_init_bin(t_shell *shell);
void		ft_add_to_the_bin(void *waste, t_type type, t_bin *bin);
void		ft_empty_the_bin(t_bin *bin);

/* free_utils */

bool		ft_freestr(char *str);
void		ft_free_redirs(t_red *redirs);
void		ft_free_tmpfile(char *hd_name, int fd);
void		ft_free_tree(t_head *tree);
void		ft_free_str_array(char **strarr);

/* free_utils2.c */
void		ft_free_env(t_shell *shell);
void		ft_filter(t_shell *shell, t_filter to_free);

/* error_utils */
bool		ft_error(char *arg, t_error error, t_shell *shell);

/** --------------- MAIN.c -----------------**/

bool		ft_get_input(t_shell *shell);

/** -------------- PARSING---------------- **/

/* parser */

t_syntax	ft_issyntax(char c);
t_syntax	ft_istoken(char c);
bool		ft_parse(t_shell *shell);
int			ft_end_size(char *input, int *end, int size, int token);
void		ft_manage_paths(t_shell *shell);

/* parse_utils1 */

bool		ft_write_pipe(t_shell *shell, t_elem *cur, char *input, int *i);
bool		ft_write_redir(t_shell *shell, t_elem *cur, char *input, int *i);
bool		ft_write_no(t_elem *cur, char *input, int *i);
void		ft_write_path(t_shell *shell, t_elem *cur);
bool		ft_write_elem(t_shell *shell, int *i);

/* parse_utils2 */

bool		ft_error_pipe_or_redir(char *to_parse);
bool		ft_unclosedquotes(char *to_parse);
bool		ft_isempty(char *input);

/* parse_utils3 */

void		ft_add_elem(t_shell *shell, t_elem *cur);
char		*ft_write_current_av(char *input, int *start);
void		ft_write_cmd(t_elem *cur, char *input, int *start);

/* parse_utils4 */

bool		ft_is_a_builtin(char *isbltn);
char		*init_path(t_shell *shell, t_elem *cur);
int			ft_array_len(char **array);
char		*ft_strcpy(char *src);
char		*ft_joinpath(char *s1, char *s2);

/* parse_utils5 */
t_red		*ft_write_lst_redir(t_red *crn, t_elem *cur, char *input, int *i);
void		ft_manage_av(char **av);
void		ft_quote(char *input, int *i, int *quote);
void		ft_line_in_hd(char *line, int fd);
bool		ft_else_open_hd(t_elem *cur, int passage_nb, t_shell *shell);

/* parse_fds*/
bool		ft_manage_fds(t_shell *shell);

/** --------------- LEXING ----------------- **/
/* lexer.c */

void		ft_end_remove_quotes(t_elem *cur, int index, char *new);
void		ft_remove_quotes(t_elem *cur, int index, int start, int *end);
void		ft_rewrite(t_shell *shell, t_elem *cur, int index, int *jindex);
char		*ft_returnenv(char *cmd, int index);
void		ft_lex(t_shell *shell);

/* lexer_utils1 */

char		*ft_removedollard(char *cmd, int size, int index);
char		*ft_replacedollard(char *cmd, char *env, int size, int *index);
char		*ft_get_status(int status);
void		ft_rewrite_dollard(t_shell *shell, t_elem *cur, int index,
				int *jindex);

/* lexer_utils2 */

void		ft_lex_av(t_shell *shell, t_elem *tmp);
bool		ft_continue(char next, int *j);

/* signals*/

/* parent's */

t_shell		*ft_getshell(t_shell *shell, int trigger);
void		ft_signals_inparent(void);
void		ft_ctrlc_inparent(int sig);

/* children's */

void		ft_signals_inchildren(void);
void		ft_ctrlc_inchildren(int sig);
void		ft_ctrlbs_inchildren(int sig);
int			ft_signal_return_handler(int sig);
void		ft_recast_return(t_shell *shell);

/* heredocs' */
void		ft_signals_inhd(void);
void		ft_ctrlc_inhd(int sig);
void		ft_ctrlbs_inhd(int sig);
int			ft_ctrld_inhd(t_elem *cur, t_red *red, int save);

/** EXECUTION **/
bool		ft_strcmp(char *cmd, char *cmp);

/* exec_utils.c */
bool		check_bltn(t_elem *cur);
void		ft_close_pipes(t_shell *shell);
void		ft_init_pipes(t_shell *shell);
int			ft_wait_children(t_shell *shell);

/* fds_utils.c */
void		ft_free_hd(t_shell *shell);
int			ft_close_fds(t_shell *shell, t_elem *cur);
int			dup_no_pipe(t_shell *shell, t_elem *cur, int i);
int			dup_pipe_rd(t_shell *shell, int i);
int			dup_pipe_wr(t_shell *shell, int i);

/* start_exec */

bool		ft_isbltn(t_shell *shell, t_elem *cur, int pid);
void		ft_exec_final(t_elem *cur, t_shell *shell);
int			ft_execve(t_shell *shell, t_elem *cur, int i);
int			ft_exec(t_shell *shell, t_elem *tmp);

/** REDIRS **/

/* redir.c */
bool		ft_simpledirright(t_elem *tmp, t_red *red, t_shell *shell);
bool		ft_doubledirright(t_elem *tmp, t_red *red, t_shell *shell);
bool		ft_simpleleftdir(t_elem *tmp, t_red *red, t_shell *shell);
bool		ft_is_eof(char *eof, char *str);
bool		ft_heredoc(t_shell *shell, t_elem *cur, t_red *red);
int			ft_redir(t_shell *shell, t_elem *cur);

/** BUILT-IN **/

void		ft_putstr_fd(char *str, int fd);
void		ft_putchar_fd(char c, int fd);
bool		ft_cmpbegin(char *cmd, char *cmp);

/* cd */

bool		ft_chdir2(t_shell *shell);
bool		ft_chdir(t_shell *shell, char *path, int pid);
bool		ft_cd(t_shell *shell, t_elem *cur, int pid);

/* echo */

void		ft_print(t_shell *shell, t_elem *cur, int i, bool no_ligne);
void		ft_echo(t_shell *shell, t_elem *cur, int pid);
void		ft_exitbltn(t_shell *shell, int status);

/* env */

void		ft_env(t_shell *shell, t_elem *cur, int pid);

/* exit */

bool		ft_atoi(char *nbr, int *result);
bool		ft_exit(t_elem *cur, t_shell *shell, int pid);

/* export */

bool		ft_export(t_shell *shell, t_elem *cur, int pid);
bool		ft_simple_export(t_shell *shell, t_elem *cur, int pid);

/* pwd */

bool		ft_pwd(t_shell *shell, t_elem *cur, int pid);

/* unset */

void		ft_unset(t_shell *shell, t_elem *cur, int pid);

/** UTILS **/

char		**ft_split(char const *s, char c);

/* libft.c */
bool		ft_is_eof(char *eof, char *str);
char		*ft_strjoin(char *s1, char *s2);
size_t		ft_countchar(int n);
char		*ft_itoa(int n, t_shell *shell);

/* diverses_utils */

int			ft_strlen(char *str);
bool		ft_isspace(char c);
bool		ft_ishell(char *input);
t_syntax	ft_returnredir(char *input, int *i);
int			ft_nbrlen(int n);

/* titl_utils */

char		*ft_smblock(void);
char		*ft_border(void);
char		**ft_av(void);
char		*ft_path(void);
void		ft_print_titl(t_shell *shell);
char		*ft_toilet(void);
char		*ft_minialfred(void);
char		*ft_option1(void);
char		*ft_option2(void);

/* ENV */

/* env_utils */

int			ft_index(char **env, char *target);
char		*ft_getenv(t_env *env, char *target);
bool		ft_update_var(t_env *env, char *to_update, char *new);
bool		ft_end_delete_var(int j, char **new_env, t_shell *shell);
bool		ft_delete_var(t_shell *shell, char *to_delete);

/* env.c */
void		ft_deletepaths(t_shell *shell);
void		ft_updatepaths(t_shell *shell);
bool		ft_setbasicenv(t_shell *shell);

#endif
