from flask import Flask, request, jsonify

from model.schema import Schema
from service.card_service import CardService

app = Flask(__name__)
cardService = CardService()


@app.route("/")
def welcome():
    return "Welcome"


@app.route("/cards", methods=["POST"])
def create_card():
    return jsonify(cardService.create(request.get_json()))


@app.route("/cards", methods=["GET"])
def get_cards():
    return jsonify(cardService.get_all())

@app.route("/cards", methods=["DELETE"])
def delete_card():
    return jsonify(cardService.delete(request.get_json()))

if __name__ == "__main__":
    Schema()
    app.run(debug=True)
