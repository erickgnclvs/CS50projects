answer = input("Do you agree? ").lower()

if answer in ["y", "yes"]:
    print(f"agreed.")
elif answer in ["n", "no"]:
    print(f"dont agree.")
