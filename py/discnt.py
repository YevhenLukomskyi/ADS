import sys


def main():
    input_filename = "discnt.in" if len(sys.argv) == 1 else sys.argv[1]
    output_filename = "discnt.out" if len(sys.argv) == 1 else sys.argv[2]

    prices, discount = read_input(input_filename)
    min_total_price = solve(prices, discount)
    write_output(output_filename, min_total_price)


def read_input(file_name):
    with open(file_name, 'r') as f:
        first_line = f.readline()
        second_line = f.readline()

        prices = [int(i) for i in first_line.split()]
        discount = int(second_line)
        return prices, discount


def is_greater(a, b):
    return a > b


def partial_sort(array, n):
    array_len = len(array)

    for i in range(0, n):
        max_index = i
        for j in range(i, array_len):
            if is_greater(array[j], array[max_index]):
                max_index = j
        array[i], array[max_index] = array[max_index], array[i]


def solve(prices, discount):
    total_price = 0.0
    n = len(prices) / 3
    partial_sort(prices, n)

    discount = 1 - discount / 100.0

    for i in range(0, n):
        total_price += prices[i] * discount

    for i in range(n, len(prices)):
        total_price += prices[i]

    return total_price


def write_output(file_name, min_total_price):
    with open(file_name, 'w') as f:
        f.write('%.2f' % min_total_price)


if __name__ == "__main__":
    main()