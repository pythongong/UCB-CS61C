from framework import Test, TestSpec, Task, Matrix, randint


@Test(seed=100)
def test_example(test: TestSpec):
    test.add_task(Task(Matrix.random(100, 99), Matrix.random(10, 9)))

@Test()
def my_custom_test(test: TestSpec):
    # rows_a = randint(15, 25)
    # cols_a = randint(15, 25) 
    # rows_b = randint(5, 15)
    # cols_b = randint(5, 15)
    # print(rows_a, cols_a)
    # print(rows_b, cols_b)
    # test.add_task(
    #     Task(Matrix.random(rows_a, cols_a), Matrix.random(rows_b, cols_b))
    # )
    test.add_task(Task(Matrix.random(4, 4, min_value=1, max_value=10),
                  Matrix.random(2, 2, min_value=1, max_value=10)))
        