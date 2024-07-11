.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
    addi sp sp -48
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw s10 40(sp)
    sw ra 44(sp)
    
    li t0 5
    bne a0 t0 argu_error
    
    # Read pretrained m0
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    # malloc row int pointer for m0
    jal malloc_int
    mv a1 a0
    mv s0 a0
    
    # malloc col int pointer for m0
    addi sp sp -4
    sw a1 0(sp)
    
    jal malloc_int
    mv a2 a0
    mv s1 a0
    
    lw a1 0(sp)
    addi sp sp 4
    
    # call read matrix for m0
    lw t0 0(sp)
    lw a0 4(t0)
    jal read_matrix
    mv s2 a0
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    
    # Read pretrained m1
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
   
    # malloc row int pointer for m1
    jal malloc_int
    mv a1 a0
    mv s3 a0
    
    # malloc col int pointer for m1
    addi sp sp -4
    sw a1 0(sp)
    
    jal malloc_int
    mv a2 a0
    mv s4 a0
    
    lw a1 0(sp)
    addi sp sp 4
    
    # call read matrix for m1
    lw t0 0(sp)
    lw a0 8(t0)
    jal read_matrix
    mv s5 a0
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    # Read input matrix
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    # malloc row int pointer for input
    jal malloc_int
    mv a1 a0
    mv s6 a0
    
    # malloc col int pointer for input
    addi sp sp -4
    sw a1 0(sp)
    
    jal malloc_int
    mv a2 a0
    mv s7 a0
    
    lw a1 0(sp)
    addi sp sp 4
    
    # call read matrix for input
    lw t0 0(sp)
    lw a0 12(t0)
    jal read_matrix
    mv s8 a0
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    

    # Compute h = matmul(m0, input)
    # alloctae memory for h
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    lw t0 0(s0)
    lw t1 0(s7)
    mul a0 t0 t1
    slli a0 a0 2
    jal malloc
    beq a0 x0 malloc_error
    mv s9 a0
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
   
    # call matmul for h
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    mv a0 s2
    lw a1 0(s0)
    lw a2 0(s1)
    mv a3 s8
    lw a4 0(s6)
    lw a5 0(s7)
    mv a6 s9
    jal matmul

    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    # Compute h = relu(h)
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    mv a0 s9
    lw t0 0(s0)
    lw t1 0(s7)
    mul a1 t0 t1
    jal relu
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    
    # Compute o = matmul(m1, h)
    # alloctae memory for o
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    lw t0 0(s3)
    lw t1 0(s7)
    mul a0 t0 t1
    slli a0 a0 2
    jal malloc
    beq a0 x0 malloc_error
    mv s10 a0
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8

    # call matmul for o
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    mv a0 s5
    lw a1 0(s3)
    lw a2 0(s4)
    mv a3 s9
    lw a4 0(s0)
    lw a5 0(s7)
    mv a6 s10
    jal matmul

    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    # Write output matrix o
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)

    lw a0 16(a1)
    mv a1 s10
    lw a2 0(s3)
    lw a3 0(s7)
    jal write_matrix
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    # Compute and return argmax(o)
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    mv a0 s10
    lw t0 0(s3)
    lw t1 0(s7)
    mul a1 t0 t1
    jal argmax
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    # If enabled, print argmax(o) and newline
    beq a2 x0 print_argmax
    
free_matrix:
    # free all alocated memory
    addi sp sp -4
    sw a0 0(sp)
    
    # free s0
    mv a0 s0
    jal free
    
    # free s1 
    mv a0 s1
    jal free
    
    # free s2
    mv a0 s2
    jal free
    
    # free s3
    mv a0 s3
    jal free
    
    # free s4
    mv a0 s4
    jal free
    
    # free s5
    mv a0 s5
    jal free
    
    # free s6
    mv a0 s6
    jal free
    
    # free s7 
    mv a0 s7
    jal free
    
    # free s8
    mv a0 s8
    jal free
    
    # free s9
    mv a0 s9
    jal free
    
    # free s10
    mv a0 s10
    jal free
    
    lw a0 0(sp)
    addi sp sp 4

    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw s10 40(sp)
    lw ra 44(sp)
    addi sp sp 48
    
    jr ra

malloc_error:
    li a0 26
    j exit

argu_error:
    li a0 31
    j exit

malloc_int:
    addi sp sp -4
    sw ra 0(sp)
    
    li a0 4
    jal malloc
    beq a0 x0 malloc_error
    
    lw ra 0(sp)
    addi sp sp 4
    jr ra

print_argmax:
    addi sp sp -4
    sw a0 0(sp)
    jal print_int
    li a0 '\n'
    jal print_char
    lw a0 0(sp)
    addi sp sp 4
    j free_matrix