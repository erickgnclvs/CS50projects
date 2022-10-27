from cs50 import get_int

mine = 2
points = get_int("How many points did you lose? ")

if points < mine:
    print(f"You lost fewer points than me.")
elif points > mine:
    print(f"You lost more points than me.")
else:
    print(f"You lost the same number of points as me.")