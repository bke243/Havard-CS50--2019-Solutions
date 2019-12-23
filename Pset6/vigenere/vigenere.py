# import some function in modules
from sys import argv
from cs50 import get_string


# kecheck function
def key_check(text):
    for i in text:
        if str.isalpha(i):
            pass
        else:
            return False
    return True


# shift function to keep track of the case
def shift(char):
    if str.isupper(char):
        return ord(char) - 65

    else:
        return ord(char) - 97


# check if two command line were entered
if len(argv) == 2:
    # check the key and get its length
    if key_check(argv[1]) == True:

        key = argv[1]
        len_key = len(key)
        index_key = 0

        # get the plaintext from the user
        text = get_string("plaintext: ")

        # cipher the plaintext
        print("ciphertext: ", end="")
        for i in text:
            if str.isalpha(i):
                # ciphering key c_key
                c_key = shift(key[index_key % len_key])

                # print ciphered char c_char
                if str.isupper(i):
                    c_char = ((ord(i) - 65) + c_key) % 26 + 65
                    print(chr(c_char), end="")
                else:
                    c_char = ((ord(i) - 97) + c_key) % 26 + 97
                    print(chr(c_char), end="")

                index_key += 1
            else:
                print(i, end="")
        print()
    else:
        print("Usage: python vigenere.py k")
        exit(1)
else:
    print("Usage: python vigenere.py k")
    exit(1)