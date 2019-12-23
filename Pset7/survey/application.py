import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


# check if a string can be converted into an integer
def is_integer(num):
    'verify if string can be converted into an integer'
    try:
        int(num)
        return True
    except ValueError:
        return False


# get the input entered by the user
def get_value(input_keys):
    'Get inputed values from the user'
    values = []
    keys = input_keys
    for i in keys:
        user_input = request.form.get(i)
        values.append(user_input)

    return values


@app.route("/form", methods=["POST"])
def post_form():

    error_messages = {"firstname": "You must specify your firstname", "lastname": "You must specify your lastname",
                      "email": "You must specify your Email", "school": "You must specify your School",
                      "degree": "You must specify your Degree", "country": "You must specify your Country",
                      "pesel": "You must specify your Pesel number", "date": "You must specify your Birthday",
                      "gender": "You must specify your Gender"}

    for key in error_messages:
        value = request.form.get(key)
        # check if the user provided all input
        if not value:
            return render_template("error.html", message=error_messages[key])
        # check the validity of the email (if "@" and "." are in the email provided by the user)
        if key == "email" and ("@" not in value or "." not in value):
            return render_template("error.html", message=error_messages[key])
        # check if the first or lastname provided by the user was/were correct
        if (key == "firstname" or key == "lastname") and is_integer(value) == True:
            return render_template("error.html", message=error_messages[key])

    # first get keys and values
    fieldnames = ['Firstname', 'Lastname', 'Email', 'School', 'Degree', 'Country', 'Pesel', 'Birthday', 'Gender']
    keys = list(error_messages.keys())
    User = get_value(keys)

    # check if the user was already registered
    with open('survey.csv', 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        registered_user = []

        for row in reader:
            List_1 = []
            for data in fieldnames:
                List_1.append(row[data])
            registered_user.append(List_1)

    # if the user was registered it show a message
    if User in registered_user:
        return render_template("error.html", message="You are already registered !")

    # then write the User info to the csv file if he is not in the csv database
    with open('survey.csv', 'a', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writerow({'Firstname': User[0], 'Lastname': User[1], 'Email': User[2], 'School': User[3],
                         'Degree': User[4], 'Country': User[5], 'Pesel': User[6], 'Birthday': User[7], 'Gender': User[8]})

    return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    'Read survey.csv and display the data display the content via sheet.html'
    fieldnames = ['Firstname', 'Lastname', 'Email', 'School', 'Pesel', 'Gender']
    with open("survey.csv", 'r', newline='') as csvfile:
        reader = csv.DictReader(csvfile)

        students = []
        numbers = []
        registered_number = 1
        for row in reader:
            firstname = row['Firstname']
            lastname = row['Lastname']
            email = row['Email']
            school = row['School']
            pesel = row['Pesel']
            gender = row['Gender']
            students.append([firstname, lastname, email, school, pesel, gender])
            numbers.append(registered_number)
            registered_number += 1

    return render_template("sheet.html", students=students, numbers=numbers)
