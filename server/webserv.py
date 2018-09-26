from flask import Flask
app = Flask(__name__)

@app.route("/")
def color():
	file = open("string.txt", "r")	
	return file.read()
