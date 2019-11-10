from flask import Flask

from model.schema import Schema
from resource.card_lock_resource import card_lock_resource
from resource.card_resource import card_resource
from resource.lock_resource import lock_resource

app = Flask(__name__)
app.register_blueprint(card_resource)
app.register_blueprint(lock_resource)
app.register_blueprint(card_lock_resource)


@app.route("/")
def welcome():
    return "Welcome"


if __name__ == "__main__":
    Schema()
    app.run(debug=True)
