# import functions in some modules
from crypt import crypt
from sys import argv


# check function
def check(word, key, password):
    if crypt(word, key) == password:
        print(word)
        exit(0)


# check the number of command-line
if len(argv) == 2:

    # string of possible letters
    letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

    # get the hash password
    password = argv[1]

    # get the salt
    salt = password[:2]

    # check for one char
    for i in letters:
        word = i
        check(word, salt, password)

    # check for two characters
    for i in letters:
        for j in letters:
            word = i+j
            check(word, salt, password)

    # check for three characters
    for i in letters:
        for j in letters:
            for k in letters:
                word = i+j+k
                check(word, salt, password)

    # check for four letters password
    for i in letters:
        for j in letters:
            for k in letters:
                for l in letters:
                    word = i+j+k+l
                    check(word, salt, password)

else:
    print("USage: python crack.py hash")
    exit(1)