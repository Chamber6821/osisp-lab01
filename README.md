# dirwalk

# Build

```shell
$ make
```
or

```shell
$ make app
```

Executable will be located by path `build/main`

# Build and Run

```shell
$ make run WF="-fs" WD=.git
```

`WF` - Work Flags (console argument) for program. Defailt: (empty)

`WD` - Work Directory for program. Default: `.` (current directory)

# Build in Debug mode

Use `MODE=debug` to add debug symbols to executable

```shell
$ make MODE=debug
```

Executable will be located by path `build/main-debug`
