from random import sample # for tests

class FibonacciCoefficient:
    def __init__(self, x0, x1):
        self.__x0 = x0
        self.__x1 = x1

    def is_unit(self):
        return (self.__x0, self.__x1) == (0, 1)

    @staticmethod
    def unit():
        return FibonacciCoefficient(0, 1)
    
    def evaluate_from(self, y0, y1):
        return self.__x0 * y0 + self.__x1 * y1

    def multiply_with(self, other):
        self.__x0, self.__x1 = (self.__x0 * (other.__x0 + other.__x1) + self.__x1 * other.__x0,
            self.__x0 * other.__x0 + self.__x1 * other.__x1)

def compute_power(x, y):
    if y == 0:
        return FibonacciCoefficient.unit()
    if x.is_unit() or y == 1:
        return x
    
    res = FibonacciCoefficient.unit()
    while True:
        if y % 2 != 0:
            res.multiply_with(x)
            if y == 1:
                return res
        x.multiply_with(x)
        y //= 2

def compute_fibonacci(i):
    res = compute_power(FibonacciCoefficient(1, 0), abs(i)).evaluate_from(1, 0)
    if i < 0 and i % 2 == 0:
        res *= -1
    return res

def main():
    for i in sample(range(200, 2000), 50):
        print("F{} = {}".format(i, compute_fibonacci(i)))

if __name__ == "__main__":
    main()
