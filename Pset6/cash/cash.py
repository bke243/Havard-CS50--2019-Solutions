# import two functions
from cs50 import get_float

# prompt user for an ampunt of change and check its validity
while True :
    change = get_float("change owed :")
    if change > 0 :
        break

# convert the change
change = round(change * 100)

# compute the number of coins
dimes = change // 25
quaters = (change % 25) // 10
nickels = ((change % 25) % 10) // 5
pennis = (((change % 25) % 10) % 5)

coins = dimes + quaters + nickels + pennis
print(coins)