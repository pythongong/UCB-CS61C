import math

from framework import Test, TestSpec, Task, Matrix, randint


@Test()
def test_tiny(test: TestSpec):
    test.add_task(Task(Matrix.random(2, 2, min_value=1, max_value=10),
                  Matrix.random(2, 2, min_value=1, max_value=10)))
    test.add_task(Task(Matrix.random(4, 4, min_value=1, max_value=10),
                  Matrix.random(2, 2, min_value=1, max_value=10)))
     
    test.add_task(Task(Matrix.random(5, 4, min_value=1, max_value=10),
                  Matrix.random(4, 3, min_value=1, max_value=10)))


@Test()
def test_small(test: TestSpec):
    for _ in range(10):
        rows_a = randint(15, 25)
        cols_a = randint(15, 25)
        rows_b = randint(5, 15)
        cols_b = randint(5, 15)
        test.add_task(
            Task(Matrix.random(rows_a, cols_a), Matrix.random(rows_b, cols_b))
        )


@Test()
def test_large(test: TestSpec):
    for _ in range(50):
        rows_a = randint(100, 100)
        cols_a = randint(100, 100)
        rows_b = randint(20, 40)
        cols_b = randint(20, 40)
        test.add_task(
            Task(Matrix.random(rows_a, cols_a, min_value=-(2 ** 30), max_value=2 ** 30),
                 Matrix.random(rows_b, cols_b, min_value=-(2 ** 30), max_value=2 ** 30))
        )


# Autograder tests
@Test(seed=61)
def test_ag_random(test: TestSpec):
    for _ in range(25):
        rows_a = randint(25, 500)
        cols_a = randint(25, 500)
        rows_b = randint(math.ceil(rows_a / 4), math.ceil(rows_a / 2))
        cols_b = randint(math.ceil(cols_a / 4), math.ceil(cols_a / 2))
        test.add_task(
            Task(Matrix.random(rows_a, cols_a), Matrix.random(rows_b, cols_b))
        )


@Test(seed=61)
def test_ag_increasing(test: TestSpec):
    for i in range(2, 29):
        rows_a = randint(math.ceil(1.25**i), math.ceil(1.25 ** (i + 1)))
        cols_a = randint(math.ceil(1.25**i), math.ceil(1.25 ** (i + 1)))
        rows_b = randint(math.ceil(rows_a / 4), math.ceil(rows_a / 2))
        cols_b = randint(math.ceil(cols_a / 4), math.ceil(cols_a / 2))
        test.add_task(
            Task(Matrix.random(rows_a, cols_a), Matrix.random(rows_b, cols_b))
        )


@Test(seed=61)
def test_ag_decreasing(test: TestSpec):
    for i in range(28, 1, -1):
        rows_a = randint(math.ceil(1.25**i), math.ceil(1.25 ** (i + 1)))
        cols_a = randint(math.ceil(1.25**i), math.ceil(1.25 ** (i + 1)))
        rows_b = randint(math.ceil(rows_a / 4), math.ceil(rows_a / 2))
        cols_b = randint(math.ceil(cols_a / 4), math.ceil(cols_a / 2))
        test.add_task(
            Task(Matrix.random(rows_a, cols_a), Matrix.random(rows_b, cols_b))
        )


@Test(seed=61)
def test_ag_big_and_small(test: TestSpec):
    test.add_task(Task(Matrix.random(1000, 1000), Matrix.random(10, 10)))
    for _ in range(500):
        rows_a = randint(50, 100)
        cols_a = randint(50, 100)
        rows_b = randint(25, 50)
        cols_b = randint(25, 50)
        test.add_task(
            Task(Matrix.random(rows_a, cols_a), Matrix.random(rows_b, cols_b))
        )


@Test(seed=61)
def test_ag_edge_1d(test: TestSpec):
    for _ in range(25):
        cols_a = randint(25, 5000)
        cols_b = randint(math.ceil(cols_a / 4), math.ceil(cols_a / 2))
        test.add_task(Task(Matrix.random(1, cols_a), Matrix.random(1, cols_b)))


@Test(seed=61)
def test_ag_edge_1d_matrix_b(test: TestSpec):
    for _ in range(25):
        rows_a = randint(25, 500)
        cols_a = randint(25, 500)
        cols_b = randint(math.ceil(cols_a / 4), math.ceil(cols_a / 2))
        test.add_task(Task(Matrix.random(rows_a, cols_a), Matrix.random(1, cols_b)))


@Test(seed=61)
def test_ag_edge_1x1(test: TestSpec):
    for _ in range(10):
        test.add_task(Task(Matrix.random(1, 1), Matrix.random(1, 1)))


@Test(seed=61)
def test_ag_edge_1x1_matrix_b(test: TestSpec):
    for _ in range(10):
        rows_a = randint(25, 100)
        cols_a = randint(25, 100)
        test.add_task(Task(Matrix.random(rows_a, cols_a), Matrix.random(1, 1)))

