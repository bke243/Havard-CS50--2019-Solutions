from cs50 import SQL


# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

borrow = db.execute("SELECT SUM(amount) FROM loan WHERE id = :user_id AND operation = :operation",
                            user_id=13, operation="borrow")

if not borrow[0].get("SUM(amount)"):
    print("not")
else:
    print("yes")

for row in borrow:
    print(row)


