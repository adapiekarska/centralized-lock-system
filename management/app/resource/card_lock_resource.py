from flask import jsonify, request, Blueprint

from service.card_lock_service import CardLockService

cardLockService = CardLockService()
card_lock_resource = Blueprint('card_lock_resource', __name__)


@card_lock_resource.route("/cards_locks", methods=["POST"])
def create_card():
    return jsonify(cardLockService.create(request.get_json()))


@card_lock_resource.route("/cards_locks", methods=["GET"])
def get_cards():
    return jsonify(cardLockService.get_all())


@card_lock_resource.route("/cards_locks", methods=["DELETE"])
def delete_card():
    return jsonify(cardLockService.delete(request.get_json()))