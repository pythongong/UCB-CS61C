.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    li t0 1
    blt a1 t0 relu_error
    li t0 0
    mv t1 a0

loop_start:
    bge t0 a1 loop_end
    lw t2 0(t1)
    blt t2 zero relu_negative_case
loop_continue:
    addi t1 t1 4
    addi t0 t0 1
    j loop_start
relu_negative_case:
    sw zero 0(t1)
    j loop_continue

relu_error:
    li a0 36 
    j exit

loop_end:
    # Epilogue

    jr ra
