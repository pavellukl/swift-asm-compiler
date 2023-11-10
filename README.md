# Installation
1. Install gcc
2. For running tests you need to install [Criterion](https://github.com/Snaipe/Criterion). Good video [here](https://www.youtube.com/watch?v=p-gi6ukMBPY).
3. `git clone git@github.com:Limikul/IFJ.git`

# Compiling and running
| Command                   | Info                                                      |
| ------------------------- | --------------------------------------------------------- |
| `make`                    | `make build`                                              |
| `make build`              | Compiles production build files                           |
| `make run [ARGS]`         | Runs production binary                                    |
|||
| `make debug`              | `make debug_build debug_run`                              |
| `make debug_build`        | Compiles development build files                          |
| `make debug_run [ARGS]`   | Runs development binary                                   |
|||
| `make test`               | `make test_build test_run`                                |
| `make test_build`         | `make build` + Compiles test files                        |
| `make test_run`           | Runs test binaries                                        |
|||
| `make deploy`             | Creates a deployment zip file and performs content check |
|||
| `make clean`              | Removes generated folders                                 |

[ARGS] -> ARGS="arg1 arg2 arg3 ..."

# Naming conventions (GTK+)
| Type                                                         | Example                                            |
| ------------------------------------------------------------ | -------------------------------------------------- |
| Macros and constants (capslock)                              | MAX_BUFFER_SIZE, TRACKING_ID_PREFIX                |
| Struct names and typedef's (camelcase)                       | GtkWidget, TrackingOrder                           |
| Functions that operate on structs (classic C style)          | gtk_widget_show(), tracking_order_process()        |
| Pointers                                                     | GtkWidget *foo, TrackingOrder *bar                 |
| Global variables                                             | Just don't use global variables. They are evil.    |
| Service functions (one or more underscores at the beginning) | _refrobnicate_data_tables(), _destroy_cache()      |
