from cs50 import get_int

n = get_int("n: ")

# If number divided by two is zero
if n % 2 == 0:
    print(f"even")
else:
    print(f"odd")