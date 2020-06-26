"""
Assignment:     Assignment 5 - Portfolio Project
Author:         Richard Nguyen
Date:           3/11/2020
Description:    Spell Checking application that detects if a word is typed incorrectly and
                subsequently uses Levenshtein distance to suggest words.
"""

from time import perf_counter


def levenshteinDistance(string1, string2):
    """
    Calculate the minimum edit distance between two given strings
    :param string1 
    :param string2 
    :return: editDistances[-1] -- int of minimum edit distance between strings
    """
    # smaller string is string1. Switch if not the case
    if len(string1) > len(string2):
        string1, string2 = string2, string1
    editDistances = range(len(string1) + 1)
    for idx2, char2 in enumerate(string2):
        newDistances = [idx2 + 1]
        for idx1, char1 in enumerate(string1):
            if char1 == char2:
                newDistances.append(editDistances[idx1])
            else:
                newDistances.append(1 + min((editDistances[idx1], editDistances[idx1 + 1], newDistances[-1])))
        editDistances = newDistances
    return editDistances[-1]


def loadDictionary():
    """
    Load a dictionary txt file into program
    :return: dictionary -- hash set of dictionary words
    """

    dictionary = set()
    with open("dictionary.txt") as inputFile:
        t_start = perf_counter()
        for word in inputFile:
            word = word.strip()
            dictionary.add(word.lower())
        t_finish = perf_counter()

        runtime = t_finish - t_start
        print("Took ", runtime, " seconds to load dictionary.")

    return dictionary


dictionary = loadDictionary()
while True:
    userInput = input("Enter a word or type 'Quit' to quit: ")
    # user quits
    if userInput == "quit" or userInput == "Quit":
        quit()

    # lists to hold the ints of 5 smallest edit distances
    suggestedDistances = []
    # list to hold the suggested word respective to the 5 shortest distances
    suggestedWords = []

    # If word is in the dictionary and spelled correctly
    if userInput.lower() in dictionary:
        print("The inputted word ", userInput, " is spelled correctly!")
    # word not found, calculate levenshtein distances
    else:
        print("The word", userInput, "is not in this dictionary!")
        for word in dictionary:
            if len(word) - len(userInput) <= 1 and len(userInput) - len(word) <= 1:
                wordDistance = levenshteinDistance(userInput, word)
                # initial 5 words
                if len(suggestedDistances) < 5:
                    suggestedDistances.append(wordDistance)
                    suggestedWords.append(word)
                # check if edit distance is smaller than one of 5 words
                else:
                    for i in range(5):
                        if wordDistance < suggestedDistances[i]:
                            suggestedDistances[i] = wordDistance
                            suggestedWords[i] = word
                            break

        print("Did you mean one of the following?: ")
        for suggestedWord in suggestedWords:
            print(" - ", suggestedWord)
