# Minishell - Creating a bash-like shell

[![C Programming](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Makefile](https://img.shields.io/badge/Build-Make-brightgreen.svg)](https://www.gnu.org/software/make/)
<!-- Add other badges if relevant, e.g., license, 42 badge -->

This is a custom Unix shell implementation created as part of the 42 school curriculum. This project focuses on understanding and recreating core shell functionalities like command parsing, execution, redirection, pipes, and process management in C.

---

## Detailed Blog Post

For a comprehensive explanation of the project, the design choices, challenges faced, concepts learned, and detailed implementation insights (including our custom garbage collector), please read the accompanying blog post on my website:

**[Writing a bash-like shell in C](https://alimnaqvi.com/blog/minishell)**

---

## Features

Our Minishell implements a subset of bash features, including:

*   **Interactive Prompt:** Displays a prompt (`minishell$`) and accepts user input.
*   **Command History:** Uses the GNU Readline library to allow navigation through previously entered commands using arrow keys.
*   **Parsing & Tokenization:**
    *   Splits input lines into commands and arguments based on spaces.
    *   Correctly handles single (`'`) and double (`"`) quotes, preventing splitting within them.
    *   Expands environment variables (`$VAR`, `$?`) outside single quotes.
    *   Recognizes and separates metacharacters (`|`, `<`, `>`, `>>`, `<<`).
*   **Command Execution:**
    *   Locates and executes external commands using the `PATH` environment variable.
    *   Executes commands in child processes using `fork` and `execve`.
*   **Built-in Commands:**
    *   `echo` (with `-n` option)
    *   `cd` (relative/absolute paths, handles `HOME`, updates `PWD`/`OLDPWD`)
    *   `pwd`
    *   `export` (adds/updates environment variables, lists them without args)
    *   `unset` (removes environment variables)
    *   `env` (lists environment variables)
    *   `exit` (terminates the shell, supports exit status argument)
*   **Redirections:**
    *   Input redirection (`<`)
    *   Output redirection (`>`)
    *   Output redirection append (`>>`)
    *   Here Documents (`<<`) using temporary files.
*   **Pipelines:** Supports multiple commands chained with pipes (`|`), managing inter-process communication.
*   **Signal Handling:** Handles `SIGINT` (`Ctrl-C`) and `SIGQUIT` (`Ctrl-\`) appropriately depending on the shell's state (interactive, running command, heredoc).
*   **Exit Status:** Correctly sets and allows access to the exit status of the last command (`$?`).
*   **Custom Garbage Collector:** Manages `malloc`'d memory and open file descriptors to prevent leaks and double frees/closes.

---

## Usage

**Prerequisites:**

*   A C compiler (`gcc` or `clang`)
*   `make`
*   GNU Readline library (e.g., `libreadline-dev` on Debian/Ubuntu, `readline` via Homebrew on macOS)

**Compilation & Execution:**

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/alimnaqvi/minishell.git
    cd minishell
    ```
2.  **Compile the project:**
    ```bash
    make
    ```
3.  **Run the shell:**
    ```bash
    ./minishell
    ```

---

## Key Concepts Applied

This project provided hands-on experience with fundamental Unix and C programming concepts:

*   **Process Management:** `fork()`, `execve()`, `waitpid()`, parent/child process interaction.
*   **File Descriptor Management:** `open()`, `close()`, `dup2()`, understanding `stdin`/`stdout`/`stderr`.
*   **Inter-Process Communication:** Using `pipe()` for command pipelines.
*   **Input/Output Redirection:** Manipulating file descriptors to redirect I/O streams.
*   **Signal Handling:** Using `sigaction()` / `signal()` to catch and handle signals like `SIGINT` and `SIGQUIT`.
*   **Parsing & Lexing:** Breaking down user input into meaningful tokens and interpreting command structure.
*   **Memory Management:** Manual allocation (`malloc()`) and deallocation (`free()`), and the implementation of a custom garbage collector.
*   **Environment Variables:** Accessing (`getenv()`) and managing the environment.
*   **Using External Libraries:** Integrating and using the GNU Readline library.
*   **Makefile:** Automating the build process.
*   **Version Control & Collaboration:** Using Git, GitHub, and Notion for project management and teamwork.

---

## License

This project was completed as part of the 42 school curriculum. Please adhere to 42 Network's policies regarding code sharing and usage.
