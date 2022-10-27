# Todo
import cs50

# Prompt user for input
    # get_string
text = cs50.get_string("Text: ")

# Defining
words = 1
letters = 0
sentences = 0

# Iterate:
for i in text:

    # Number of words
    if i == " ":
        words += 1

    # Number of letters
    elif i.isalpha():
        letters += 1

    # Number of sentences
    elif i == "." or i == "!" or i == "?":
        sentences += 1

# Apply formula
    # 0.0588 * L - 0.296 * S - 15.8
index = 0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8
index = round(index)

# Result
    # If index < 1: Before Grade 1
if index < 1:
    print("Before Grade 1")

    # If index > 16: Grade 16+
elif index >= 16:
    print("Grade 16+")

    # Else: Grade {index}
else:
    print(f"Grade {index}")