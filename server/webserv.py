from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello():
	return "Hello World!"

@app.route("/r")
def r():
	file = open("string.txt", "r")	
	return file.read()

@app.route("/g")
def g():
	return '100'

@app.route("/b")
def b():
	return '255'
