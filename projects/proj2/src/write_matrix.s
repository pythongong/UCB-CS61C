.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

    # Prologue
    addi sp sp -8
    sw s0 0(sp)
    sw ra 4(sp)
    
    mul s0 a2 a3
    
    # open file
    addi sp sp -12
    sw a1 0(sp)
    sw a2 4(sp)
    sw a3 8(sp)
    
    li a1 1
    jal fopen
    li t0 -1
    beq a0 t0 fopen_error
    
    lw a1 0(sp)
    lw a2 4(sp)
    lw a3 8(sp)
    addi sp sp 12
    
    # write rows number
    addi sp sp -16
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    
    li a1 0
    addi a1 sp 8
    li a2 1
    li a3 4
    jal fwrite
    li t0 1
    bne a0 t0 fwrite_error
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a3 12(sp)
    addi sp sp 12
    
    # write columns number
    addi sp sp -12
    sw a0 0(sp)
    sw a1 4(sp)
    sw a3 8(sp)
    
    li a1 0
    addi a1 sp 8
    li a2 1
    li a3 4
    jal fwrite
    li t0 1
    bne a0 t0 fwrite_error
    
    lw a0 0(sp)
    lw a1 4(sp)
    addi sp sp 16
    
    # write matrix bytes
    addi sp sp -4
    sw a0 0(sp)
    
    mv a2 s0
    li a3 4
    jal fwrite
    bne a0 s0 fwrite_error
    
    lw a0 0(sp)
    addi sp sp 4
    

    # close the file
    jal fclose
    li t0 -1
    beq a0 t0 fclose_error


    # Epilogue
    lw s0 0(sp)
    lw ra 4(sp)
    addi sp sp 8

    jr ra

fopen_error:
    li a0 27
    j exit

fclose_error:
    li a0 28
    j exit
    
fwrite_error:
    li a0 30
    j exit