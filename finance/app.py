import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)


# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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

    user_id = session['user_id']

    # grab transactions from database
    transactions_db = db.execute('SELECT symbol, SUM(shares) AS shares, price FROM transactions WHERE user_id = ? GROUP BY symbol', user_id)

    # grab information from transactions

    for row in transactions_db:

        row['name'] = lookup(row['symbol'])
        row['price'] = round(lookup(row['symbol'])['price'], 2)
        row['total'] = round(lookup(row['symbol'])['price'] * row['shares'], 2)

    investment_total = 0

    for row in transactions_db:
        investment_total += row['total']

    investment_total = round(investment_total, 2)

    # grab cash from database
    cash_db = db.execute('SELECT cash FROM users WHERE id = ?', user_id)
    cash = round(float(cash_db[0]['cash']), 2)
    grand_total = round(investment_total + cash, 2)

    return render_template('index.html', transactions_db=transactions_db, cash=cash, investment_total=investment_total, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # first handles get methods
    if request.method == 'GET':
        return render_template('buy.html')

    # then handles post methods
    else:
        symbol = request.form.get('symbol')
        shares = request.form.get('shares')

        # error checking
        if not shares.isdigit():
            return apology("cant buy partial shares")

        shares = int(shares)

        if not symbol:
            return apology('no symbol provided')

        stock = lookup(symbol.upper())

        if not stock:
            return apology('symbol doesnt exist')

        if not shares or shares < 0:
            return apology('provide a number bigger than 1')

        # check for price of transaction
        transaction_value = shares * stock['price']

        # grab users cash from database
        user_id = session['user_id']
        user_cash_db = db.execute('SELECT cash FROM users WHERE id = :id', id = user_id)

        # extract int from the obtained dict
        user_cash = user_cash_db[0]['cash']

        # check if user has money
        if user_cash < transaction_value:
            return apology("can't afford")

        # update users table with correct ammount of cash
        new_cash_ammount = user_cash - transaction_value
        db.execute('UPDATE users SET cash = ? WHERE id = ?', new_cash_ammount, user_id)

        # create new transaction
        date = datetime.datetime.now()
        db.execute('INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?);', user_id, stock['symbol'], shares, stock['price'], date)

        flash('bought!')
        return redirect('/')

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session['user_id']
    transactions_db = db.execute('SELECT * FROM transactions WHERE user_id = ?', user_id)
    return render_template('history.html', transactions=transactions_db)

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

    # first get to show the page
    if request.method == 'GET':
        return render_template('quote.html')

    # then post to interact with backend
    else:
        symbol = request.form.get('symbol')

        # check for errors
        if not symbol:
            return apology('no symbol provided')

        # search for stock in api an store in a variable
        stock = lookup(symbol.upper())

        if not stock:
            return apology('symbol doesnt exist')

        # display information of the stock variable
        return render_template('quoted.html', name = stock['name'], price = stock['price'], symbol = stock['symbol'])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # first do get method, to just display the register page
    if request.method == 'GET':
        return render_template('register.html')

    # then do post method, to submit users input
    elif request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        # check for problems
        if not username or not password or not confirmation:
            return apology('please fill all spaces')

        if password != confirmation:
            return apology('passwords dont match')

        # submit input
        securepassword = generate_password_hash(password)

        # check if username not in database
        try:
            newsession = db.execute('INSERT INTO users (username, hash) VALUES (?, ?);', username, securepassword)

        except:
            return apology('username already exists :(')

        session['user_id'] = newsession

        return redirect('/')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # first handles get methods
    if request.method == 'GET':
        user_id = session['user_id']
        symbols_user = db.execute('SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0', user_id)
        return render_template('sell.html', symbols = [row['symbol'] for row in symbols_user])

    # handles post
    else:
        symbol = request.form.get('symbol')
        shares = int(request.form.get('shares'))

        # error checking
        if not symbol:
            return apology('no symbol provided')

        stock = lookup(symbol.upper())

        if not stock:
            return apology('symbol doesnt exist')

        if shares < 0:
            return apology('provide a number bigger than 1')

        # check for price of transaction
        transaction_value = shares * stock['price']

        # grab users cash from database
        user_id = session['user_id']
        user_cash_db = db.execute('SELECT cash FROM users WHERE id = :id', id = user_id)

        # extract int from the obtained dict
        user_cash = user_cash_db[0]['cash']

        # check ammount of shares user has
        user_shares_db = db.execute('SELECT shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol', user_id, symbol)
        user_shares = user_shares_db[0]['shares']
        if shares > user_shares:
            return apology('you dont own all this')

        # update users table with correct ammount of cash
        new_cash_ammount = user_cash + transaction_value
        db.execute('UPDATE users SET cash = ? WHERE id = ?', new_cash_ammount, user_id)

        # create new transaction
        date = datetime.datetime.now()
        db.execute('INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?);', user_id, stock['symbol'], (-1)*shares, stock['price'], date)

        flash('sold!')
        return redirect('/')

@app.route('/changepassword', methods=['GET', 'POST'])
@login_required
def changepassword():
    """user will be able to change password"""

    # first handle get methods
    if request.method == "GET":

        return render_template('changepassword.html')

    # handle post methods

    else:

        password = request.form.get('password')
        newpassword = request.form.get('newpassword')
        confirmation = request.form.get('confirmation')

        # check for problems
        if not password or not newpassword or not confirmation:
            return apology('please fill all spaces')

        if newpassword != confirmation:
            return apology('passwords dont match')

        # Query database for username
        user_id = session['user_id']
        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Ensure password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid password", 403)

        # secure input
        securepassword = generate_password_hash(newpassword)

        # update database
        # UPDATE table_name SET column1 = value1, column2 = value2, ... WHERE condition;

        db.execute('UPDATE users SET hash = ? WHERE id = ?', securepassword, user_id)

        flash('password updated!')
        return redirect('/')