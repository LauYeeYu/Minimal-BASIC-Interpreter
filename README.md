# Minimal BASIC Interpreter - 2021

This is an assignment of SJTU ACM class, a minimal BASIC interpreter. For more information about this assignment, click [here](https://github.com/ACM-Programming-2021/BASIC-Interpreter).

此爲上海交通大學 ACM 班程式設計大作業之一，一個精簡的 BASIC 解釋器。如需獲取更多關於此大作業的内容，請點擊[此處](https://github.com/ACM-Programming-2021/BASIC-Interpreter)。



This minimal BASIC interpreter, written with C++ language, can interpret some statement instantly, and also can  store and run a program in ascending order. Besides, this interpreter uses `StanfordCPPLib`, which has the problem of memory leak, so it may probably have the same problem.

這個以 C++ 編寫的解釋器既可以立即解釋某些語句並執行，又可以編寫一個按行數升序依次執行的大型程式。 鑒於此解釋器使用了`StanfordCPPLib`（其存在記憶體流失問題），此解釋器亦可能存在此問題。





## How to Use 使用方法

You can use the `CMakeLists.txt` to compile this project.

你可以使用 `CMakeLists.txt` 來編譯此項目。



In this interpreter, three statements (`LET`, `PRINT`, and `INPUT`) can be executed both instantly and in a program. Program statements (`RUN`, `LIST`, `CLEAR`, `QUIT`, `HELP`) is used to control the program to operate. Other statements (`REM`, `END`, `GOTO`, `IF ... THEN`) can only be executed in a program.

對於此解釋器，`LET`、`PRINT`、 `INPUT` 三個指令可以即時或在大型程式中執行。控制指令 (`RUN`, `LIST`, `CLEAR`, `QUIT`, `HELP`) 則被用於控制大型程式的運作。其餘指令 (`REM`, `END`, `GOTO`, `IF ... THEN`) 則只可在大型程式中執行。



You can type things in these syntax:

你可以以如下語法輸入：

```basic
// Sequential Statements
REM <anything>                    // Comments
LET <var> = <exp>                 // Initializer
PRINT <exp>                       // Print expression
INPUT <var>                       // Identifier setter
END                               // Indicate the end of program

// Control Statements
GOTO <num>                        // jump to a certain line
IF <exp> <cmp> <exp> THEN <num>   // GOTO <num> if the former one is true

// Program statements
RUN                               // Excute the program
LIST                              // List all lines in program
CLEAR                             // Clear program and every identifier
QUIT                              // Exit the program
HELP                              // To give some help
```

For more detail, please look up the `Minimal BASIC Interpreter - 2021.pdf`.

如需瞭解更多，請參見`Minimal BASIC Interpreter - 2021.pdf`



