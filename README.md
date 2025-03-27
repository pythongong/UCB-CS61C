# UCB-CS61C

All projects as well as lab 01-08 are from the summer 2024 course. Lab 09-10 are from the fall 2022 course.  Course website: 
- [https://cs61c.org/su24 ](https://web.archive.org/web/20240827122147/https://cs61c.org/su24/)

# Projoect 1: snek

This project creates a snake game by C. I need to modify two files -- state.c and snake.c in the src folder. 

I rarely wrote C codes in the past. It's most diffcult for me to access C string at the beginning. It took me a lot of time to find a bug caused by not adding '\0' to the end of a string. 

I was very uncomfortable with allocating memory manually at the beginning. When I got a segment fault, I didn't even know which line of codes cause this bug. I had to make a guess and used gdb to find where the bug lies. 

However, I finally overcame all these difficulties and passed all local tests. I'm more familiar with C now.

You can use `make run-unit-tests` to run the provided unit tests (which are not comprehensive). To test the full implementation, run `make run-integration-tests`.

# Project 2: CS61Classify
In this project, I write RISC-V code to implement some matrix operations. The codes I wrote are the in src folder.

This is the first time that I write a project by assembly code. Calling functions is a very tedious process in RISC-V. It was easy for me to ignore some parts of calling conventions. 

I'm now familiar with RISC-V including load, store, jump and branch instructions.

This project uses the tool Venus prodived by the course. You can enter the project folder and download it by the following commands
```
bash tools/download_tools.sh
```

Then you can test it 
```
bash test.sh all
```

# Project 3: CS61CPU
In this project, I use the software in Logisim to build a two-stage CPU that runs actual RISC-V instructions. You can enter the project folder and download Logisim by the following commands
```
bash tools/download_tools.sh
```
Then you can run it by Java and see the circuit diagram.
```
java -jar tools/logisim-evolution.jar
```

I mapped all the circuit diagrams in the cpu folder except mem.circ. This is the hardes project in this course in my opnion. And the hardest part of this project is to implement the control logic. It took me the longest time to finish it.

I have to change my ways of thinking when dealing with the circuit diagrams as I can't map the digrams by if/else. 

I understand how a cpu works by this project.

To run the test:
```
python3 tools/run_test.py tests/

python3 tools/run_test.py --pipelined tests/
```

# Project 4: CS61kaChow
In this project, I write C code to convolute 2-D matrices first. Then I use SIMD instructions and OpenMP to optimize the code. At last, I use Oopen MPI to make the program multi-processes.

The is optimization is very obvious when using SIMD instructions and OpenMP. But multi-processes only have the advantage when convoluting more than 1, 000 matrices. If the data set is small, the multi-processes program is even slower than the naive program as the process is so heavy.

To test the naive program
```
make task_1 TEST=tests/test_tiny
make task_1 TEST=tests/test_small
make task_1 TEST=tests/test_large

make task_1 TEST=tests/my_custom_test
```

To test the multi-threads program
```
make task_2 TEST=tests/test_tiny
make task_2 TEST=tests/test_small
make task_2 TEST=tests/test_large

make task_2 TEST=tests/my_custom_test
```

To test the multi-processes program

```
make task_3 TEST=tests/test_tiny
make task_3 TEST=tests/test_small
make task_3 TEST=tests/test_large

make task_3 TEST=tests/my_custom_test
```

# Labs

Lab 1 and 2 are for C programming. Lab 3 and 4 are for RSIC-V. Lab 5 and 6 are for CPU. Lab 7 is for SIMD instructions. Lab 8 is for OpenMP. Lab 9 is for caches. Lab 10 is for virtual memory which uses the tool Camera

```
java -jar tools/Camera.jar
```
