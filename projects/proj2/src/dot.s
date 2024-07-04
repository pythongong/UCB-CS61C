.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

    # Prologue
    li t0 1
    blt a2 t0 length_error
    blt a3 t0 stride_error
    blt a4 t0 stride_error
    li t0 4
    mul a3 a3 t0
    mul a4 a4 t0
    li t0 0
    mv t1 a0
    li a0 0

loop_start:
    bge t0 a2 loop_end
    lw t2 0(t1)
    lw t3 0(a1)
    mul t3 t3 t2
    add a0 a0 t3
    add t1 t1 a3
    add a1 a1 a4
    addi t0 t0 1
    j loop_start
loop_end:

    # Epilogue


    jr ra
length_error:
    li a0 36 
    j exit
stride_error:
    li a0 37
    j exit