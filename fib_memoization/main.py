import time
from typing import TypedDict, List
from matplotlib import pyplot as plt



def recursive_fibonacci(n, counter):
    counter["recursive_fib"] += 1
    if n == 0 or n == 1:
        value = n
    else:
        value = recursive_fibonacci(n - 1, counter) + recursive_fibonacci(n - 2, counter )
    return value

def memoized_fibonacci(n, fibtable, counter):
    counter["memoized_fib"] += 1
    if n in fibtable:
        return fibtable[n]
    elif n == 0 or n == 1:
        value = n
    else:
        value = memoized_fibonacci(n - 1, fibtable, counter) + memoized_fibonacci(n - 2, fibtable, counter)

    fibtable[n] = value
    return value

def main():
    fibtable = {0: 0, 1: 1}

    print("####################################### \n","PART A:")
    for n in range(20):
        print("====================== \n", "N = ",n)
        counter = {"recursive_fib": 0, "memoized_fib": 0}
        print("Plain Recursion: ", recursive_fibonacci(n,counter))
        print("Memoized Recursion: ", memoized_fibonacci(n, fibtable,counter))

    print("####################################### \n", "PART B:")
    class ResultTuple(TypedDict):
        n: int
        calls: int
        time: float
    print("====================== \nPlain Recursion: ",)
    recur_results: list[ResultTuple] = []
    for n in range(5, 35,5):
        print( "N = ", n)
        start = time.perf_counter()
        counter = {"recursive_fib": 0, "memoized_fib": 0}
        print("n_fib: ", recursive_fibonacci(n,counter))
        end = time.perf_counter()
        _time = float(end - start)
        result = ResultTuple(n = n,calls=counter["recursive_fib"],time = _time)
        recur_results.append(result)
        if _time > 60: break
    print("LARGEST n ACHIEVED: ", recur_results[-1]['n'])

    print("====================== \nMemoized Recursion: ", )

    mem_results: list[ResultTuple] = []

    for n in [10,50,100,200,500]:
        print("N = ", n)
        fibtable = {0: 0, 1: 1}
        counter = {"recursive_fib": 0, "memoized_fib": 0}
        start = time.perf_counter()
        print("n_fib: ", memoized_fibonacci(n,fibtable, counter))
        end = time.perf_counter()
        _time = float(end - start)
        result = ResultTuple(n = n,calls=counter["memoized_fib"],time = _time)
        mem_results.append(result)

    print("Memoized Fibonacci: ", mem_results)
    print("Recursive Fibonacci: ", recur_results)
    print("###################################### \n", "PART C:")


if __name__ == "__main__":
    main()