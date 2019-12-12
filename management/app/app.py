from flask import Flask
from flask_cors import CORS

from model.schema import Schema
from resource.auth_mapping_resource import auth_mapping_resource
from resource.token_resource import token_resource
from resource.lock_resource import lock_resource
from resource.access_resource import access_resource

app = Flask(__name__)
CORS(app)
app.register_blueprint(token_resource)
app.register_blueprint(lock_resource)
app.register_blueprint(auth_mapping_resource)
app.register_blueprint(access_resource)


@app.route("/")
def welcome():
    return "Welcome"

if __name__ == "__main__":
    Schema()
    app.run(debug=True)
