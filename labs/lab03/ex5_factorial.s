.globl factorial

.data
n: .word 0

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

# factorial takes one argument:
# a0 contains the number which we want to compute the factorial of
# The return value should be stored in a0
factorial:
    # YOUR CODE HERE
    addi t1 t1 1
Loop:
    bge x0 a0 End
    mul t1 t1 a0
    addi a0 a0 -1
    j Loop
End:
    mv a0 t1
    jr ra
