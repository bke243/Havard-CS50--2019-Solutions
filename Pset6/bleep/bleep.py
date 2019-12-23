from cs50 import get_string
from sys import argv


def main():

    # check the number of command-line arguments
    check(argv)

    # dictionary
    dictionary = argv[1]

    # load the dictionary
    List = load(dictionary)

    # get text fro the user
    text = get_string("What message would you like to censor?\n").split()

    # print the message
    display(text, List)
    print()




# function check
def check(argv):
    dictionary = argv[1]
    if len(argv) == 2 :
        pass
    else:
        print("Usage: python bleep.py dictionary")
        exit(1)

def load(dictionary):
    infile = open(dictionary, "r")
    List = infile.readlines()
    infile.close()
    List_1  = [i.split("\n")[0] for i in List]
    return List_1

# to dispaly the word
def display(list_1, list_2):
    for i in list_1:
        if str.lower(i) in list_2:
            print("*" * (len(i)), end=" ")
        else:
            print(i, end=" ")

if __name__ == "__main__":
    main()