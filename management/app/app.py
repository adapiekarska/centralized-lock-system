from flask import Flask
from flask_cors import CORS

from model.schema import Schema
from resource.card_lock_resource import card_lock_resource
from resource.card_resource import card_resource
from resource.lock_resource import lock_resource

app = Flask(__name__)
CORS(app)
app.register_blueprint(card_resource)
app.register_blueprint(lock_resource)
app.register_blueprint(card_lock_resource)

@app.route("/")
def welcome():
    return "Welcome"
# Try to port this app to virtual env or some other way so it can be deployed
# easily
# Install CORS packages -`pip install -U flask-cors`

if __name__ == "__main__":
    Schema()
    app.run(debug=True)
