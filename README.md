# MySh - A Custom Unix Shell
Joshua C. Ticot | Estefen Emerson R. Contreras

A robust, modular Unix shell implemented in C, featuring advanced command parsing, I/O redirection, and background job management.

---

## 1. Compilation and Usage

This project includes a **Makefile** to streamline the build process and manage dependencies.

### **Build the shell**
To compile the source code and generate the `mysh` executable:
```bash
make all
```

### **Run the Shell**
To launch the shell environment after compiling:
```bash
make run
```

### **Clean Build Files**
To clean all the built files for recompilation:
```bash
make clean
```

## 2. Implemented Features

* **Basic Command Execution**: Runs external programs found in the system `$PATH` (e.g., `ls`, `grep`, `top`).
* **Built-in Commands**:
    * `cd`: Change directories.
    * `pwd`: Display the current working directory.
    * `exit`: Gracefully exit the shell.
    * `history`: Display the last 10 commands entered.
* **I/O Redirection**:
    * **Input redirection**: `cat < file.txt`
    * **Output redirection (Truncate)**: `ls > output.txt`
    * **Output redirection (Append)**: `echo "hello" >> output.txt`
* **Advanced Parsing**: Supports commands without spaces around operators (e.g., `ls>out.txt` or `sleep 10&`).
* **Background Jobs**: Execute commands in the background using the `&` operator. Includes an automatic "Reaping" system to notify the user when background tasks finish.
* **Validation**: Pre-fork validation ensures background jobs are only created for valid commands, preventing "ghost" jobs from cluttering the job list.

---

## 3. Architecture Overview & Design Decisions

The shell is designed with a **Context-Command architecture** to ensure modularity and memory safety.



* **Modular Design**: The logic is split into `main.c` (loop), `parser.c` (string manipulation), `executor.c` (process management), and `builtins.c` (internal commands).
* **Command Struct**: Instead of passing raw strings to the executor, the parser populates a `Command` struct. This makes it easy to handle complex metadata like redirection filenames and background flags in one place.
* **Pre-fork Validation**: A key design decision was to search the `$PATH` manually before calling `fork()`. This prevents the shell from printing "Job Started" messages for commands that do not exist.
* **Circular History**: History is stored in a fixed-size array within the `ShellContext`, ensuring low memory overhead and $O(1)$ access.

---

## 4. Known Limitations & Bugs

* **Piping**: Currently, this version does not support multi-stage pipes (e.g., `ls | grep .c`).
* **Signal Handling**: Pressing `Ctrl+C` will terminate the entire shell rather than just the foreground process.
* **Environment Variables**: While it uses `$PATH` to find commands, it does not support custom environment variable expansion (e.g., `echo $USER`).
* **Reaping Delay**: Background job completion messages appear only when the user presses `Enter` or runs a subsequent command, as the `reap_background_jobs` function is tied to the main execution loop.

