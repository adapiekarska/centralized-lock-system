from flask import jsonify, request, Blueprint

from service.card_service import CardService

cardService = CardService()
card_resource = Blueprint('card_resource', __name__)


@card_resource.route("/cards", methods=["POST"])
def create_card():
    return jsonify(cardService.create(request.get_json()))


@card_resource.route("/cards", methods=["GET"])
def get_cards():
    return jsonify(cardService.get_all())


@card_resource.route("/cards", methods=["DELETE"])
def delete_card():
    return jsonify(cardService.delete(request.get_json()))