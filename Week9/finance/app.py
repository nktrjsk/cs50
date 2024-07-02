import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, timestamp_to_datetime

import json
import time

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["datetime"] = timestamp_to_datetime

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    stocks = db.execute("SELECT * FROM stocks WHERE user_id=?", session["user_id"])
    user_money = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]

    stocks_worth = 0
    for stock in stocks:
        stocks_worth += stock["price"] * stock["shares"]

    return render_template("index.html", stocks=db.execute("SELECT * FROM stocks WHERE user_id=?", session["user_id"]),
                           cash=user_money, total=stocks_worth + user_money)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        stock = lookup(symbol)
        if stock == None:
            return apology("Invalid symbol")
        if not shares.isdigit():
            return apology("Invalid shares")
        shares = int(shares)

        user_info = db.execute("SELECT * FROM users WHERE id=?", session["user_id"])
        user_money = user_info[0]["cash"]

        stock_price = stock["price"]

        if stock_price * shares > user_money:
            return apology("Sorry, no money")

        db.execute("INSERT INTO stocks (user_id, shares, symbol, price) VALUES (?, ?, ?, ?);",
                   session["user_id"], shares, symbol, stock_price)
        db.execute("INSERT INTO history (user_id, type, symbol, shares, price, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], "Buy", symbol, shares, stock_price, int(time.time()))

        user_money -= stock_price * shares
        db.execute("UPDATE users SET cash = ? WHERE id=?", user_money, session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return render_template("history.html", history=db.execute("SELECT * FROM history WHERE user_id=?;", session["user_id"]))


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    stock = request.args.get("symbol")

    if stock == None:
        return render_template("quote.html"), 400
    else:
        return render_template("quote.html", stock=lookup(stock))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if username == "" or password == "" or confirmation == "":
            return apology("Please enter all information")

        if len(db.execute("SELECT * FROM users WHERE username=?;", username)) != 0:
            return apology("Username is taken")
        elif password != confirmation:
            return apology("Passwords don't match")

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", username, generate_password_hash(password))

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        stock = lookup(symbol)
        if stock == None:
            return apology("Invalid symbol")
        if not shares.isdigit() or shares == "0":
            return apology("Invalid shares")
        shares = int(shares)

        stocks = db.execute("SELECT * FROM stocks WHERE user_id = ? AND symbol = ? AND shares >= ? ORDER BY shares DESC", session["user_id"], symbol, shares)
        if len(stocks) == 0:
            return apology("Not enough shares")
        stocks = stocks[0]

        if stocks["shares"] - shares == 0:
            db.execute("DELETE FROM stocks WHERE id=?;", stocks["id"])
        else:
            db.execute("UPDATE stocks SET shares=? WHERE id=?;", stocks["shares"] - shares, stocks["id"])

        user_money = db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash=? WHERE id=?;", user_money + stocks["price"] * shares, session["user_id"])
        db.execute("INSERT INTO history (user_id, type, symbol, shares, price, timestamp) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], "Sell", symbol, shares, stocks["price"], int(time.time()))

        return redirect("/")

    else:
        symbols = db.execute("SELECT DISTINCT symbol FROM stocks WHERE user_id=?;", session["user_id"])
        symbols = [symbol["symbol"] for symbol in symbols]
        print(symbols)

        return render_template("sell.html", symbols=symbols)

@app.route("/lookup")
def lookupstock():
    stock = request.args.get("symbol")

    if stock == None:
        return "No symbol", 404

    stock = lookup(stock)
    if stock == None:
        return "No stock found", 404
    else:
        return json.dumps(stock)
@app.route("/passchange", methods=["GET", "POST"])
@login_required
def passchange():
    if request.method == "POST":
        current = request.form.get("current")
        new = request.form.get("new")
        confirmation = request.form.get("confirmation")

        current_hash = db.execute("SELECT hash FROM users WHERE id=?;", session["user_id"])[0]["hash"]
        new_hash = generate_password_hash(new)

        if new_hash == current_hash:
            return apology("Current password is wrong")
        elif new != confirmation:
            return apology("Passwords don't match")

        db.execute("UPDATE users SET hash=? WHERE id=?", new_hash, session["user_id"])

        return redirect("/")
    else:
        return render_template("passchange.html")
