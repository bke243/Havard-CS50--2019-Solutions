from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # get the lines in the string a
    List_1 = a
    List_1 = list(List_1.splitlines())

    # get the lines inthe string b
    List_2 = b
    List_2 = list(List_2.splitlines())

    # list that we will return
    List = compare(List_1, List_2)

    return List


def sentences(a, b):
    """Return sentences in both a and b"""

    # get the sentences in the string a
    List_1 = sent_tokenize(a)

    # get the sentences in the string b
    List_2 = sent_tokenize(b)

    # list that we will return
    List = compare(List_1, List_2)

    return List


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # get all possible substrings in a
    List_1 = extract_substring(a, n)

    # get all possible substrings in b
    List_2 = extract_substring(b, n)

    # list that we will return
    List = compare(List_1, List_2)

    return List


# helper function for extracting substring
def extract_substring(a, n):
    'Return a list of substrings'
    length = len(a)
    n = n
    List = []
    for i in range(length + 1 - n):
        List.append(a[i: i+n])

    return List


# compare two list and return a list with common elements
def compare(list_1, list_2):
    'Return common elements of two list'

    # list to return
    List = []

    # input lists
    list_1 = list_1
    list_2 = list_2

    for i in list_1:
        if i in list_2 and i not in List:
            List.append(i)

    return List
