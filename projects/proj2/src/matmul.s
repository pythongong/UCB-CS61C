.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

    # Error checks
    li t0 1
    blt a1 t0 matrix_error
    blt a2 t0 matrix_error
    blt a4 t0 matrix_error
    blt a5 t0 matrix_error
    bne a2 a4 matrix_error

    # Prologue
    addi sp sp -20
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw ra 16(sp)
    # initial the row counter of martix A
    li s0 0
    # initial the col counter of martix B
    li s1 0
    # initial the pointer of matrix d
    mv s2 a6
    
    slli s3 a5 2
    slli a4 a4 2

outer_loop_start:
    # if the rows of matrix A have all been traversed
    # end the program
    bge s0 a1 outer_loop_end
    j inner_loop_start

inner_loop_start:
    # if the columns of matrix B have all been traversed
    # move to the next line of matrix A
    bge s1 a5 inner_loop_end

    # call dot
    addi sp sp -24
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw a4 16(sp)
    sw a5 20(sp)
    
    mv a1 a3
    li a3 1
    # set the stride of matrix B to its row number
    mv a4 a5
    
    jal ra dot
    
    sw a0 0(s2)
    
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw a4 16(sp)
    lw a5 20(sp)
    
    addi sp sp 24
    
    # increment the start address of matrix B
    addi a3 a3 4
    # increment the pointer of matrix d
    addi s2 s2 4
    # increment the col counter of martix B
    addi s1 s1 1
    j inner_loop_start
    
inner_loop_end:
    # add one row to current index of matrix A
    add a0 a0 a4
     # increment the row counter of martix A
    addi s0 s0 1
    
    # restore the start address and col counter if matrix B
    sub a3 a3 s3
    li s1 0
    
    j outer_loop_start
    
outer_loop_end:
    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw ra 16(sp)
    addi sp sp 20
    
    jr ra

matrix_error:
    li a0 38
    j exit