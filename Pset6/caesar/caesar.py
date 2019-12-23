# import functions in modules
from cs50 import get_string
from sys import argv

# check the number of command line input
if len(argv) == 2:

    # check the key and convert it
    for i in argv[1]:
        if ord(i) < 48 or ord(i) > 57:
            print("Usage: ./caesar key")
            exit(1)

    # convert the key
    key = int(argv[1])

    # prompt user
    text = get_string("plaintext: ")
    print("ciphertext: ", end="")

    # ciphering the text and keep the case sensitivity
    for i in text:
        if str.isalpha(i):

            if str.isupper(i):
                char = ((ord(i) - 65) + key) % 26 + 65
                print(chr(char), end="")

            else:
                char = ((ord(i) - 97) + key) % 26 + 97
                print(chr(char), end="")

        else:
            print(i, end="")

    print()
else:
    print("Usage: ./caesar key")
    exit(1)