# Minishell

A minimal interactive shell written in C, inspired by **bash**. This project implements a read-eval-print loop with tokenization, parsing, environment handling, pipelines, redirections, heredocs, signal-aware execution, and a custom memory allocator with centralized cleanup.

## Features

### Interactive shell

- Prompt: `minishell `
- Line editing and history via **GNU Readline**
- Command history is preserved for the session (`add_history`)

### Lexing and parsing

- **Lexer** splits input into tokens (words, pipes, redirection operators)
- **Parser** builds a command list with arguments and a per-command **redirection** list
- **Expander** expands environment variables (`$VAR`, `$?`) and handles quote rules

### Execution

- **Pipelines** (`cmd1 | cmd2 | …`) with separate processes and connected pipes
- **External commands** resolved via `PATH` or explicit paths (when `/` is present)
- **Built-in commands** run in the parent when appropriate (see below)

### Redirections

- Input: `< file`
- Output: `> file`
- Append: `>> file`
- **Heredoc**: `<< LIMITER` (with signal handling in heredoc mode; quoted delimiters can disable expansion)

### Built-ins

| Command   | Description |
|-----------|-------------|
| `echo`    | Print arguments; supports `-n` |
| `cd`      | Change working directory |
| `pwd`     | Print current working directory |
| `export`  | Set or export environment variables |
| `unset`   | Remove variables from the environment |
| `env`     | Print the environment |
| `exit`    | Exit the shell with an optional status |

### Environment

- Environment is loaded from `envp` at startup and maintained as a linked list (`t_enviroment`)
- `export` / `unset` / `cd` update both the internal structure and the process environment as needed

### Signals

- **Interactive mode**: `Ctrl+C` (SIGINT) interrupts the current line; exit status **130** is stored when appropriate
- **Execution mode**: child processes receive appropriate signal behavior; SIGQUIT handling differs from interactive mode
- **Heredoc mode**: dedicated handlers for SIGINT during heredoc input

### Memory management

- Allocations are tracked through a small **memory layer** (`memory_malloc`, `memory_calloc`, `memory_absorb` for pointers such as `readline` output, `memory_free` for global cleanup)
- Helps avoid leaks and keeps teardown predictable

## Project layout

| Path | Role |
|------|------|
| `main.c` | REPL, signal setup, line processing |
| `parse/` | Lexer, parser, tokenizer, quotes, expansion, errors, heredoc parsing |
| `execute/` | Pipes, forks, waits, path resolution, heredoc execution helpers |
| `redirector/` | `<`, `>`, `>>`, heredoc application |
| `built_in/` | Built-in implementations |
| `env/` | Environment list and helpers |
| `signal/` | Signal setup for interactive / execute / heredoc |
| `memory/` | Centralized allocation helpers |
| `utils/` | Shared helpers |
| `libft/` | Static library (standard + bonus linked list functions) |
| `libraries/` | Public headers (`minishell.h`, `parser.h`, etc.) |
| `readline.supp` | Valgrind suppressions for libreadline (optional) |

## Requirements

- C compiler (`cc` in the Makefile)
- **GNU Readline** development libraries (`readline`, `history`)
- `make`
- POSIX-like environment (Linux, macOS, or WSL with build tools installed)

On Debian/Ubuntu-style systems, you may need:

```bash
sudo apt-get install build-essential libreadline-dev
```

## Build

```bash
make
```

This builds `libft/libft.a` (with bonus objects) and links the `minishell` binary with `-lreadline`.

## Run

```bash
./minishell
```

Type `exit` or send EOF (`Ctrl+D` on an empty line) to leave the shell.

## Clean

```bash
make fclean
```

Removes object files, `libft.a`, and the `minishell` binary.

## Optional: Valgrind

If you use Valgrind and see noise from Readline, you can pass the provided suppressions file:

```bash
valgrind --suppressions=readline.supp ./minishell
```

## Limitations

This is an educational shell, not a full **bash** clone. Behavior is aligned with the **42 Minishell** subject: focus on correctness for the required features, robust error handling, and clear process/signal semantics rather than full POSIX coverage.

## License / credits

School project (42). See source file headers for authors and dates.
