# Shell Architecture

## 1. Overview

This project implements a Unix-like command-line shell written in C.  
The shell is designed using a **modular, layered architecture** that separates input handling, parsing, execution, and system interaction. This design improves maintainability, extensibility, and clarity while closely mirroring the internal structure of real-world shells such as `bash` and `zsh`.

The shell processes user commands through a pipeline of stages: **input → lexical analysis → parsing → execution**.

---

## 2. High-Level Architecture

The shell follows a compiler-inspired architecture:

User Input
↓
Shell Core (Prompt & Loop)
↓
Lexer (Tokenization)
↓
Parser (AST Construction)
↓
Executor (Process Management)
↓
Operating System (fork / exec)


Each stage is implemented as a separate module with clearly defined responsibilities.

---

## 3. Core Components

### 3.1 Shell Core

**Location:** `src/core/`

The core module controls the shell lifecycle and overall execution flow.

**Responsibilities:**
- Displaying the shell prompt
- Reading user input
- Managing the main execution loop
- Maintaining shell state (environment, last exit status)

The shell core does not perform parsing or execution logic directly. Instead, it coordinates the interaction between components.

---

### 3.2 Context Management

**Location:** `src/core/context.c`

The shell context encapsulates global shell state in a structured form.

**Stored state includes:**
- Environment variables
- Last command exit status
- Shell running flag

This design avoids excessive use of global variables and enables easier testing and extension.

---

## 4. Parsing Subsystem

### 4.1 Lexical Analysis (Lexer)

**Location:** `src/parsing/lexer.c`

The lexer converts raw user input into a sequence of tokens.

**Examples of tokens:**
- Command words (`ls`, `grep`)
- Operators (`|`, `<`, `>`)
- Arguments (`-l`, filenames)

The lexer handles:
- Whitespace separation
- Quoted strings
- Special shell characters

---

### 4.2 Syntax Analysis (Parser)

**Location:** `src/parsing/parser.c`

The parser transforms the token stream into an **Abstract Syntax Tree (AST)**.  
The AST represents the logical structure of the command, including pipelines and redirections.

**Example AST:**

PIPE
├── COMMAND (ls -l)
└── COMMAND (grep txt)


Using an AST allows the shell to:
- Respect operator precedence
- Execute pipelines and redirections reliably
- Extend functionality cleanly

---

### 4.3 AST Management

**Location:** `src/parsing/ast.c`

This module is responsible for:
- Creating AST nodes
- Linking nodes together
- Freeing the AST after execution

---

## 5. Execution Subsystem

### 5.1 Command Executor

**Location:** `src/execution/executor.c`

The executor traverses the AST and determines how each command should be executed.

**Execution decisions include:**
- Built-in command vs external command
- Single command vs pipeline
- Foreground vs background execution

---

### 5.2 Pipeline Handling

**Location:** `src/execution/pipeline.c`

This module manages inter-process communication using Unix pipes.

**Responsibilities:**
- Creating pipes
- Forking child processes
- Redirecting standard input and output using `dup2`
- Closing unused file descriptors

---

### 5.3 Redirection Handling

**Location:** `src/execution/redirection.c`

This module handles input and output redirection.

**Supported operations:**
- Input redirection (`<`)
- Output redirection (`>`)
- Append redirection (`>>`)

It ensures proper file descriptor management and restoration.

---

### 5.4 Job Control (Optional)

**Location:** `src/execution/jobs.c`

This module supports advanced shell features such as:
- Background processes (`&`)
- Process group management
- Signal handling (`SIGINT`, `SIGTSTP`)

---

## 6. Built-in Commands

**Location:** `src/builtins/`

Built-in commands are executed directly within the shell process.

Each built-in command is implemented in a separate source file:
- `cd` – change directory
- `exit` – terminate shell
- `export` – modify environment variables
- `help` – display usage information

This separation allows easy extension and clean integration.

---

## 7. Utility Modules

**Location:** `src/utils/`

Utility modules provide shared functionality used across the shell.

**Includes:**
- String manipulation helpers
- Memory management utilities
- Unified error reporting

Centralizing these functions reduces duplication and improves consistency.

---

## 8. Error Handling Strategy

Errors are handled consistently across all modules using a unified error-reporting interface.  
The shell ensures that invalid commands or system call failures do not cause crashes and that resources are properly released.

---

## 9. Design Goals

The architecture is designed to achieve the following goals:
- Modularity and separation of concerns
- Extensibility for additional shell features
- Robust handling of user input and system calls
- Readability and maintainability

---

## 10. Summary

This shell architecture closely follows established systems programming principles and reflects the internal structure of modern Unix shells. By separating input processing, parsing, and execution, the design ensures clarity, scalability, and correctness.

