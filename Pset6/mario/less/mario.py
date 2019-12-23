# import the function get_int from cs50 module
from cs50 import get_int

# prompt and validate user input
while True:
    height = get_int("Height: ")

    # chech condition
    if height > 0 and height <= 8 and type(height) == int:
        break

# row number
row = 1

# print the pyramid
for i in range(height):
    print(" "*(height-row), end="")
    print("#"*row, end="")
    row += 1
    print()