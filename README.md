# Corvus Shell

Corvus is a Unix shell implementation in C++ using POSIX APIs. It implements 
core shell functionality including command execution, pipes, redirection, 
and signal handling, built to understand how shells work at the system level.

## Building

```bash
make
make clean
```

## Usage

```bash
./corvus
```

## Features

- [x] Command execution with `fork` and `execvp`
- [x] Built-in commands (`cd`, `pwd`, `exit`, `help`)
- [x] N command pipes
- [x] Redirections (input and output)

## Built-in Commands

| Command | Description |
|---------|-------------|
| `cd` | Change directory |
| `pwd` | Print working directory |
| `exit` | Exit the shell |
| `help` | List available built-ins |


## License

MIT