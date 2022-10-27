from cs50 import get_float

# Ask how many cents the customer is owed
while True:
    cents = get_float("Cents owed: ")
    if cents > 0:
        break

# Round cents
cents = round(cents * 100)

# Initiate number of coins
coins = 0

# Calculate number of quarters
while cents >= 25:
    cents = cents - 25
    coins = coins + 1

# Calculate number of dimes
while cents >= 10:
    cents = cents - 10
    coins = coins + 1

# Calculate number of nickels
while cents >= 5:
    cents = cents - 5
    coins = coins + 1

# Calculate number of pennies
while cents >= 1:
    cents = cents - 1
    coins = coins + 1

# Print number of coins
print(coins)