# Projekt do předmětu IFJ 2023/Project for IFJ 2023

✍️ Autoři/Authors:

- [xluklp00](mailto:xluklp00@vutbr.cz)
- [xklani00](mailto:xklani00@vutbr.cz)
- [xmilis00](mailto:xmilis00@vutbr.cz)
- [xcalko00](mailto:xcalko00@vutbr.cz)

🎨 Abstrakt/Abstract:

Tento projekt vznikl v rámci předmětu IFJ (Formální jazyky a překladače) na VUT v Brně, 2023. Jedná se o překladač napsaný v jazyce C, který překládá kód z jazyka podobnému jazyku Swift do jazyka podobnému Assembly. Překladač je rozdělen na několik částí: Skener, Parser, Precedenční Syntaktický Analyzátor a Generátor kódu. Každý zdrojový soubor obsahuje hlavičku s jménem jeho autora. / This project was created within the IFJ (Formal Languages and Compilers) course at Brno University of Technology, 2023. It is a compiler written in C that translates code from a Swift-like language to an Assembly-like language. The compiler is divided into several parts: the Scanner, the Parser, the Precedence Syntactic Analyzer and the Code Generator. Each source file contains a header with the name of its author.

# Installation
1. Install gcc
2. For running tests you need to install [Criterion](https://github.com/Snaipe/Criterion). Good video [here](https://www.youtube.com/watch?v=p-gi6ukMBPY).
3. `git clone git@github.com:jklanica/IFJ_compiler.git`

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
