# TODO
from cs50 import get_string

text = get_string("Text: ")

words = len(text.split(" ")) / 100
L = len([i for i in text if str.isalpha(i)]) / words
S = len([i for i in text if i in ".!?"]) / words

grade = 0.0588 * L - 0.296 * S - 15.8

if grade < 1:
    print("Before Grade 1")
elif grade >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(grade)}")