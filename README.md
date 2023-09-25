# Installation
1. Install gcc
2. For running tests you need to install [Criterion](https://github.com/Snaipe/Criterion). Good video [here](https://www.youtube.com/watch?v=p-gi6ukMBPY).
3. `git clone git@github.com:Limikul/IFJ.git`

# Compiling and running
| Command                   | Info                                          |
| ---------------------     | --------------------------------------------- |
| `make`                    | `make build run`                              |
| `make build`              | Compiles production build files               |
| `make run [ARGS]`         | Runs production binary                        |
|||
| `make debug`              | `make debug_build debug_run`                  |
| `make debug_build`        | Compiles development build files              |
| `make debug_run [ARGS]`   | Runs development binary                       |
|||
| `make test`               | `make test_build test_run`                    |
| `make test_build`         | `make build` + Compiles test files            |
| `make test_run`           | Runs test binaries                            |
|||
| `make clean`              | Removes generated folders                     |

[ARGS] -> ARGS="arg1 arg2 arg3 ..."