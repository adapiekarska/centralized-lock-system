from flask import jsonify, request, Blueprint

from service.token_service import TokenService

tokenService = TokenService()
token_resource = Blueprint('token_resource', __name__)


@token_resource.route("/api/tokens", methods=["POST"])
def create_token():
    return jsonify(tokenService.create(request.get_json()))


@token_resource.route("/api/tokens", methods=["GET"])
def get_tokens():
    return jsonify(tokenService.get_all())


@token_resource.route("/api/tokens", methods=["DELETE"])
def delete_token():
    return jsonify(tokenService.delete(request.get_json()))