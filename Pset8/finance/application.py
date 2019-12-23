import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)


# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get the user'id
    user_id = session.get("user_id")
    # get the necessary informaion to display
    user_info = db.execute(
        "SELECT symbol,name,shares FROM stocks_and_shares WHERE id = :user_id  ORDER BY symbol ASC", user_id=user_id)
    user_balance = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)
    CASH = usd(user_balance[0]["cash"])

    # get price of stock(s) owned by the user
    total = {}
    prices = {}
    grand_total = user_balance[0]["cash"]

    for row in user_info:
        symbol = row["symbol"]
        shares = row["shares"]
        info_symbol = lookup(symbol)
        current_price = info_symbol["price"]
        total_price = shares * current_price
        grand_total += total_price
        total[symbol] = usd(total_price)
        prices[symbol] = usd(current_price)

    total_balance = usd(grand_total)

    return render_template("index.html", user_info=user_info, CASH=CASH, prices=prices, total=total, total_balance=total_balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # if the method is GET just display the form
    if request.method == "GET":
        return render_template("buy.html")

    # if the method is POST :
    else:

        # series of check on inputs to verify if they all are valid
        if not request.form.get("symbol"):
            return apology("You must provide a Symbol")
        elif not lookup(request.form.get("symbol")):
            return apology("You must provide a valid symbol")
        elif not request.form.get("shares").isdigit():
            return apology("You must provide the number of shares")
        elif int(request.form.get("shares")) == 0:
            return redirect("/")

        # Get price of shares and available balance of the user
        user_id = session.get("user_id")
        symbol = request.form.get("symbol")
        value = request.form.get("shares")
        shares = int(value)
        row = db.execute("SELECT cash FROM users WHERE id= :user_id", user_id=user_id)
        balance = row[0]["cash"]
        info_symbol = lookup(symbol)
        price_symbol = info_symbol["price"]

        # if the balance is not enough to buy share(s)
        if balance - (shares * price_symbol) < 0:
            return apology("Sorry you don't have enough money to do this operation")
        # if the balance is enough to buy share(s)
        else:
            # insert infomation into purchase table
            db.execute("INSERT INTO purchase(id, name, share, price) VALUES(:user_id, :info_symbol,:shares, :price_symbol)",
                       user_id=user_id, info_symbol=info_symbol["symbol"], shares=shares, price_symbol=price_symbol)

            # insert information into stocks_and_shares table
            # first of all check if the user has  stock(s)
            rows = db.execute("SELECT name FROM stocks_and_shares WHERE id = :user_id ", user_id=user_id)

            # if the user has not stock(s)
            if not rows:
                db.execute("INSERT INTO stocks_and_shares(id, name, shares, symbol) VALUES(:user_id, :info_symbol, :shares, :symbol)",
                           user_id=user_id, info_symbol=info_symbol["symbol"], shares=shares, symbol=info_symbol["symbol"])
            # if the user has stock(s)
            else:

                # if the user has that stock(s)
                if {"name": info_symbol["name"]} in rows:
                    shares_owned = db.execute("SELECT shares FROM stocks_and_shares WHERE name = :info_symbol",
                                              info_symbol=info_symbol["name"])
                    current_shares = shares_owned[0]["shares"]
                    new_shares_owned = current_shares + shares
                    db.execute("UPDATE stocks_and_shares SET shares= :new_shares_owned WHERE id = :user_id AND symbol=:info_symbol",
                               new_shares_owned=new_shares_owned, user_id=user_id, info_symbol=info_symbol["symbol"])
                # if the user has not that stock(s)
                else:
                    db.execute("INSERT INTO stocks_and_shares(id, name, shares, symbol) VALUES(:user_id, :info_symbol, :shares, :symbol)",
                               user_id=user_id, info_symbol=info_symbol["name"], shares=shares, symbol=info_symbol["symbol"])

            # update the user's balance
            new_balance = balance - (shares * price_symbol)
            db.execute("UPDATE users SET cash = :new_balance WHERE id = :user_id", new_balance=new_balance, user_id=user_id)

            flash("Bought !")
            return redirect("/")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    # check if the username was  taken
    rows = db.execute("SELECT username FROM users")
    username = request.args.get("username")

    if len(username) == 0:
        return jsonify(False)
    elif {"username": username} in rows:
        return jsonify(False)
    else:
        return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # get user_id
    user_id = session.get("user_id")
    rows = db.execute("SELECT name, share, price, date FROM purchase WHERE id= :user_id", user_id=user_id)
    for row in rows:
        row["price"] = usd(row["price"])

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    else:
        symbol = request.form.get("symbol")
        value = lookup(symbol)
        if not value:
            return apology("INVALID SYMBOL")

        message = "A share of " + value["name"] + " " + "({})".format(value["symbol"]) + " costs " + usd(value["price"])
        return render_template("quoted.html", message=message)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # if the method is GET just display the register form
    if request.method == "GET":
        return render_template("register.html")

    # if the method is POST do required manipulation before storing the user into finance.db
    else:
        # check if input fields were not all blank
        fields = {"username": "Missing username!", "password": "Missing password!",
                  "confirmation": "Missing confirmation password!"}
        for key in fields:
            value = request.form.get(key)
            if not value:
                return apology(fields[key])

        # check password
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Please sure that your two passport match!")

        # check if the username was  taken
        rows = db.execute("SELECT username FROM users")
        username = request.form.get("username")

        if {"username": username} in rows:
            return apology("the Username was already taken")
        else:
            # generate the hash password and register the user into the database
            password = request.form.get("password")
            hash_password = generate_password_hash(password)
            db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash_password)",
                       username=username, hash_password=hash_password)

            # store the user's id into session for a better user experience
            user = db.execute("SELECT id  FROM users WHERE username = :username", username=username)
            session["user_id"] = user[0]["id"]

            # redirect to the home page
            flash("You are registered")
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # get user_id and symbol that he has
    user_id = session.get("user_id")
    rows = db.execute("SELECT symbol FROM stocks_and_shares WHERE id = :user_id", user_id=user_id)

    # if the method is GET display the form only :
    if request.method == "GET":
        return render_template("sell.html", rows=rows)
    # if the method is POST do some manipulations
    else:

        # check the correctness of the user input
        if not request.form.get("symbol"):
            return apology("Missing symbol")
        elif not request.form.get("shares"):
            return apology("Missing shares")
        elif not request.form.get("shares").isdigit():
            return apology("Provide a valid shares")
        elif int(request.form.get("shares")) == 0:
            return redirect("/")

        shares_to_sell = int(request.form.get("shares"))
        symbol = request.form.get("symbol")
        shares_owned = db.execute("SELECT shares FROM stocks_and_shares WHERE id=:user_id AND symbol=:symbol",
                                  user_id=user_id, symbol=symbol)
        current_shares = shares_owned[0]["shares"]

        # if the share(s) is/are not enough
        if current_shares - shares_to_sell < 0:
            return apology("You don't have enough shares to complete this transaction")

        # if the share(s) is/are enough
        else:
            info_symbol = lookup(symbol)
            price_symbol = info_symbol["price"]
            total_price = price_symbol * shares_to_sell
            cash_owned = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)
            current_cash = cash_owned[0]["cash"]
            new_cash_owned = current_cash + total_price
            # update current cash
            db.execute("UPDATE users SET cash = :new_cash_owned WHERE id = :user_id",
                       new_cash_owned=new_cash_owned, user_id=user_id)

            # insert the transaction into the purchase table
            db.execute("INSERT INTO purchase(id, name, share, price) VALUES(:user_id, :info_symbol,:shares, :price_symbol)",
                       user_id=user_id, info_symbol=info_symbol["symbol"], shares=(-1) * (shares_to_sell), price_symbol=price_symbol)

            # update the user's shares
            if (current_shares - shares_to_sell) == 0:
                db.execute("DELETE FROM stocks_and_shares WHERE id=:user_id AND symbol= :symbol", user_id=user_id, symbol=symbol)
            else:
                new_shares_owned = current_shares - shares_to_sell
                db.execute("UPDATE stocks_and_shares SET shares=:new_shares_owned WHERE id=:user_id AND symbol=:symbol",
                           new_shares_owned=new_shares_owned, user_id=user_id, symbol=symbol)

            flash("Sold!")
            return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


@app.route("/Loan", methods=["GET", "POST"])
@login_required
def Loan():
    """ Allow users to take and return a Loan"""

    # get some imoprtant values
    user_id = session.get("user_id")
    borrow = db.execute("SELECT SUM(amount) FROM loan WHERE id = :user_id AND operation = :operation",
                        user_id=user_id, operation="borrow")
    pay = db.execute("SELECT SUM(amount) FROM loan WHERE id = :user_id AND operation = :pay",
                     user_id=user_id, pay="pay")
    if request.method == "GET":
        # get list of operations
        rows = db.execute("SELECT operation,amount,date FROM loan WHERE id = :user_id", user_id=user_id)

        if not borrow[0].get("SUM(amount)") and not pay[0].get("SUM(amount)"):
            current_debt = borrowed = payed = usd(0)
        else:
            if borrow[0].get("SUM(amount)") and not pay[0].get("SUM(amount)"):
                borrowed = usd(borrow[0].get("SUM(amount)"))
                payed = usd(0)
                current_debt = usd(abs(borrow[0].get("SUM(amount)")))
            elif not borrow[0].get("SUM(amount)") and pay[0].get("SUM(amount)"):
                payed = usd(pay[0].get("SUM(amount)"))
                borrowed = usd(0)
                current_debt = usd(abs(pay[0].get("SUM(amount)")))
            else:
                payed = usd(pay[0].get("SUM(amount)"))
                borrowed = usd(borrow[0].get("SUM(amount)"))
                current_debt = usd(abs(borrow[0].get("SUM(amount)") - pay[0].get("SUM(amount)")))

        for row in rows:
            row["amount"] = usd(row.get("amount"))

        return render_template("loan.html", rows=rows, payed=payed, borrowed=borrowed, current_debt=current_debt)
    else:
        # check some_values
        if not request.form.get("operation"):
            return apology("You must select an operation")
        elif not request.form.get("amount").isdigit():
            return apology("You must provide a valud amount")
        elif int(request.form.get("amount")) == 0:
            return redirect("/Loan")

        # get the user's balance
        balance = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)
        cash = balance[0]["cash"]
        operation = request.form.get("operation")

        # get the amount
        amount = int(request.form.get("amount"))
        # check if the user has enough cash to perform the operation
        if request.form.get("operation") == "pay":
            if cash - amount < 0:
                return apology("You must have enough cash to pay your loan")
            elif not borrow[0].get("SUM(amount)"):
                return apology("You do not have a debt")
            elif borrow[0].get("SUM(amount)") and not pay[0].get("SUM(amount)"):
                if amount > borrow[0].get("SUM(amount)"):
                    return apology("You must refund less or exactly what you borrowed")
            elif borrow[0].get("SUM(amount)") and pay[0].get("SUM(amount)"):
                if borrow[0].get("SUM(amount)") - pay[0].get("SUM(amount)") == 0:
                    return apology("You have not a debt")
                elif amount > borrow[0].get("SUM(amount)") - pay[0].get("SUM(amount)"):
                    return apology("You must refund less or exactly what you borrowed")

        #  insert data into the loan table
        db.execute("INSERT INTO loan(id, operation, amount) VALUES(:user_id, :operation, :amount)",
                   user_id=user_id, operation=operation, amount=amount)

        # update the amount of cash in the database
        db.execute("UPDATE users SET cash = :new_cash WHERE id= :user_id", new_cash=cash + amount, user_id=user_id)
        # flash messeage
        if operation == "borrow":
            flash("Borrowed")
        else:
            flash("Refunded")
        return redirect("/Loan")