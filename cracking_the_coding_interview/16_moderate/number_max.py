# write a method that finds the maximum of two numbers. You should not use
# if else or any other comparison operator

# should be some sort of arithmatics that automatically gives back max
def num_max(a, b):
    return (a+b+abs(a-b)) // 2


if __name__ == "__main__":
    print(num_max(10, -5), 10)
    print(num_max(25, 21), 25)
    print(num_max(12, -1221), 12)
