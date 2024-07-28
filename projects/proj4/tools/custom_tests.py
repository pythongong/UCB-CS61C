from framework import Test, TestSpec, Task, Matrix, randint


@Test(seed=100)
def test_example(test: TestSpec):
    test.add_task(Task(Matrix.random(100, 99), Matrix.random(10, 9)))

@Test()
def my_custom_test(test: TestSpec):
    for _ in range(5000):
        rows_a = randint(100, 100)
        cols_a = randint(100, 100)
        rows_b = randint(20, 40)
        cols_b = randint(20, 40)
        test.add_task(
            Task(Matrix.random(rows_a, cols_a, min_value=-(2 ** 30), max_value=2 ** 30),
                 Matrix.random(rows_b, cols_b, min_value=-(2 ** 30), max_value=2 ** 30))
        )
        