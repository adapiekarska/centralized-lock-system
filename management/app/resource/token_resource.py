from flask import jsonify, request, Blueprint

from service.auth_mapping_service import AuthMappingService
from service.token_service import TokenService

tokenService = TokenService()
authMappingService = AuthMappingService()
token_resource = Blueprint('token_resource', __name__)


@token_resource.route("/api/tokens", methods=["POST"])
def create_token():
    return jsonify(tokenService.create(request.get_json()))


@token_resource.route("/api/tokens", methods=["GET"])
def get_tokens():
    return jsonify(tokenService.get_all())


@token_resource.route("/api/tokens/<_id>", methods=["GET"])
def get_token(_id):
    token = tokenService.get_token(_id)[0]
    locks = authMappingService.get_all(f"lock_id={_id}")
    response = {
        "id": token["id"],
        "locks": locks
    }
    return jsonify(response)


@token_resource.route("/api/tokens/<_id>", methods=["DELETE"])
def delete_token(_id):
    return jsonify(tokenService.delete(_id))