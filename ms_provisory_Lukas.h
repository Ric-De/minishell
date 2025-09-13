Puede ser q cambio algunos de los struts despues, pero de memento just imagine that they're are they are below


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lroberts <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:07:34 by lroberts          #+#    #+#             */
/*   Updated: 2025/09/12 12:14:47 by lroberts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: don't forget to remove this
#ifdef DEBUG
# define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
# define DEBUG_PRINT(fmt, ...)
#endif

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                           SYSTEM INCLUDES                                 */
/* ========================================================================== */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <stdarg.h>
# include <stdint.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

/* TODO: update libft with the one in campus */
# include "./libft/libft.h"

/* ========================================================================== */
/*                              DEFINES                                      */
/* ========================================================================== */

// == minishell.h   ==============
# define MAX_PROMPT 1024

// == expander.h    ==============
# define GLOB_SUCCESS 0
# define GLOB_NO_MATCH 0
# define GLOB_ERROR -1

/* ========================================================================== */
/*                           ENUMERATIONS                                    */
/* ========================================================================== */

// == lexer.h       ==============
typedef enum e_token_type
{
    T_NONE = 0,
    T_WRD,
    T_LESS,
    T_GREAT,
    T_DLESS,
    T_DGREAT,
    T_PIPE,
    T_O_PARENTH,
    T_C_PARENTH,
    T_AND,
    T_OR,
    T_NL,
    T_SNG_QUOTE,
    T_DBL_QUOTE,
    T_EOF
}   t_token_type;

typedef enum e_lexer_state
{
    IN_WRD,
    IN_SNG_QUOTE,
    IN_DBL_QUOTE,
    IN_PARENTH,
    IN_ERROR
}   t_lexer_state;

// == ast_construction.h    ==============
typedef enum e_ast_node_type
{
    N_NONE,
    N_CMD,
    N_COMPOUND_CMD,
    // OTHER
}   t_ast_node_type;

// == other_src.h       ==================
typedef enum e_shell_err_type
{
    SH_ER_NONE = 0,
    SH_ER_MEM,
    SH_ER_UNMATCHED_PAIR,
    SH_ER_OTHER_SYNTAX
    // SH_ER_OPEN_DBL_QUOTE,
    // SH_ER_OPEN_SNGL_QUOTE,
    // SH_ER_OPEN_PARENT
}   t_shell_err_type;

// == garbage_collection.h  ============
/* Assuming this enum exists based on g_collector function signature */
typedef enum e_g_collecter_flg
{
    O_MALLOC,
    O_FREE,
    O_FREE_ALL,
    O_REALLOC
} t_g_collecter_flg;

/* ========================================================================== */
/*                            STRUCTURES                                     */
/* ========================================================================== */

/* From lexer.h */
typedef struct s_token
{
    t_token_type        type;
    const char              *start; // inclusive
    const char              *end;   // exclusive
    struct s_token      *next;
    // struct s_token   *prev; // I think we can remove
}   t_token;

typedef struct s_state_machine
{
    t_lexer_state               state;
    // struct s_state_machine   *above;
    struct s_state_machine      *below;
}   t_state_machine;

typedef struct s_word_range
{
    size_t  i;
    size_t  start;
    size_t  end;
    // int  escape_count;
}   t_word_range;

/* From ast_construction.h */
typedef struct s_ast_node
{
    t_ast_node_type     type;
    struct s_ast_node   *right;
    struct s_ast_node   *left;
}   t_ast_node;

/* From expander.h */
typedef struct e_glob_result
{
    char    **matches;
    int     count;
    int     capacity;
}   t_glob_output;

/* From other_src.h */
typedef struct s_shell_err
{
    t_shell_err_type    errtype;
    int                 node_pos;
    char                *token_content;
}   t_shell_err;

/* From original minishell.h - Main shell structure */
typedef struct s_minishell
{
    char                *line;
    t_token             *lex_head;
    t_state_machine     *state_machine;
    // t_node           *ast; // not implemented yet
    t_shell_err         *shell_err;


    // t_cmd_err        cmd_err; // TODO: Ricardo implements

    // int exit_values -- some place to save the exit values of commands / shell (or should they be saved inside `t_cmd_err cmd_err`)

    char                **envp;
    int                 rainbow_offset;
    struct termios      og_term_vars; // This is from the <termios.h> lib
}   t_minishell;

// ==   garbage_collection.h    =========================
typedef struct s_mem_block
{
    void *mem;
    size_t size;
    struct s_mem_block *next;
}   t_mem_block;

/* ========================================================================== */
/*                        FUNCTION DECLARATIONS                              */
/* ========================================================================== */

/* From other_src ================== */
// main.c

// debugging.c
int     ft_error(const char *errmsg, const char *file,
            const char *function, int line);
// error_handling.c
int handle_shell_err(t_shell_err_type err_type);
void set_shell_err(t_shell_err_type shell_err_type,  int lst_ps, char *token_content);
// quit.c
void    quit_minishell(char *str, int return_value);
void    free_lex_tokens(void);
// garbage_collectiton.c
void    *g_collector(t_g_collecter_flg flg, ...);
// gc_realloc.c
void *gc_realloc(t_mem_block **head, va_list args);
size_t retrieve_alloc_size(void *mem_ptr, t_mem_block *head);
// rainbow_prompt.c
char    *readline_wrapper(void);


/* From lexer   ===================== */
// state_machine_func.c
void init_state_machine(void);
void pop_state_machine();
void push_state_machine(t_lexer_state state);
// lexer.c
int lexer(void);
int excise_token(const char *line, t_word_range *word_range, t_token **head, int build_word_flg);
t_token *lex_line(const char *line);
t_token *append_new_token(const char *line, t_token **head_token, t_token_type type, t_word_range *word_range);

/* ==== GLOBAL VARIABLE ====================================================================== */

extern t_minishell  g_minishell; // Global minishell instance

#endif /* MINISHELL_H */
