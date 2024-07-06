.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:
    # Prologue
    addi sp sp -12
    sw s0 0(sp)
    sw s1 4(sp)
    sw ra 8(sp)

    # open file
    addi sp sp -8
    sw a1 0(sp)
    sw a2 4(sp)
    
    li a1 0
    jal fopen
    li t0 -1
    beq a0 t0 fopen_error
    
    lw a1 0(sp)
    lw a2 4(sp)
    addi sp sp 8
    
    # read rows number
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    
    li a2 4
    jal fread
    li t0 4
    bne a0 t0 fread_error
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    # read cols number
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    
    mv a1 a2
    li a2 4
    jal fread
    li t0 4
    bne a0 t0 fread_error
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    # get the size of matrix
    lw t0 0(a1)
    lw t1 0(a2)
    mul s0 t0 t1
    slli s0 s0 2
    
    # malloc memory for the matrix
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    
    mv a0 s0
    jal malloc
    li t0 0
    beq a0 t0 malloc_error
    mv s1 a0
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    addi sp sp 12
    
    # read matrix bytes
    addi sp sp -4
    sw a0 0(sp)
    
    mv a1 s1
    mv a2 s0
    jal fread
    bne a0 s0 fread_error
    
    lw a0 0(sp)
    addi sp sp 4


    # close the file
    jal fclose
    li t0 -1
    beq a0 t0 fclose_error
    
    # Epilogue
    
    mv a0 s1
    lw s0 0(sp)
    lw s1 4(sp)
    lw ra 8(sp)
    addi sp sp 12
    
    jr ra

malloc_error:
    li a0 26
    j exit

fopen_error:
    li a0 27
    j exit

fclose_error:
    li a0 28
    j exit
    
fread_error:
    li a0 29
    j exit