from flask import Flask

from model.schema import Schema

app = Flask(__name__)


@app.route("/")
def hello():
    return "Hello world"


if __name__ == "__main__":
    Schema()
    app.run(debug=True)
