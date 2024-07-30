# UCB-CS61C

# [Projoect 1: snek](https://web.archive.org/web/20240301065844/https://cs61c.org/sp24/projects/proj1/)

This project creates a snake game by C. I need to modify two files -- state.c and snake.c. I rarely wrote C codes in the past. It's most diffcult for me to access C string at the beginning. It took me a lot of time to find a bug caused by not adding '\0' to the end of a string. I was very uncomfortable with allocating memory manually at the beginning. When I got a segment fault, I didn't even know which line of codes cause this bug. I had to make a guess and used gdb to find where the bug lies. However, I finally overcame all these difficulties and passed all local tests. I'm more familiar with C now.

You can use `make run-unit-tests` to run the provided unit tests (which are not comprehensive). To test the full implementation, run `make run-integration-tests`.

To debug the program, use the following commands:
- `make debug-unit-tests`
- Use `p print_board(state, stdout)` to print out your entire board while debugging in cgdb.
- `cgdb --args ./snake -i tests/TESTNAME-in.snk -o tests/TESTNAME-out.snk`
- `valgrind ./snake -i tests/TESTNAME-in.snk -o tests/TESTNAME-out.snk`

# Project 2

```
bash tools/download_tools.sh

java -jar tools/venus.jar . -dm

```

https://venus.cs61c.org/

# Project 3

```
bash tools/download_tools.sh

java -jar tools/logisim-evolution.jar

python3 tools/run_test.py tests/unit-alu/alu-add.circ

python3 tools/run_test.py --pipelined tests/

bash test.sh run_custom -p
```