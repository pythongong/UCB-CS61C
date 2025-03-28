.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    li t0 1
    blt a1 t0 argmax_error
    mv t1 a0
    lw t2 0(a0)
    li a0 0

loop_start:
    bge t0 a1 loop_end
    addi t1 t1 4
    lw t3 0(t1)
    blt t2 t3 argmax_update
    
loop_continue:
    addi t0 t0 1
    j loop_start

loop_end:
    # Epilogue

    jr ra

argmax_update:
    mv a0 t0
    lw t2 0(t1)
    j loop_continue

argmax_error:
    li a0 36 
    j exit