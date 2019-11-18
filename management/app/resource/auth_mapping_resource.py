from flask import jsonify, request, Blueprint

from service.auth_mapping_service import AuthMappingService

tokenLockService = AuthMappingService()
auth_mapping_resource = Blueprint('auth_mapping_resource', __name__)


@auth_mapping_resource.route("/api/auth_mappings", methods=["POST"])
def create_token():
    return jsonify(tokenLockService.create(request.get_json()))


@auth_mapping_resource.route("/api/auth_mappings", methods=["GET"])
def get_tokens():
    return jsonify(tokenLockService.get_all())


@auth_mapping_resource.route("/api/auth_mappings", methods=["DELETE"])
def delete_token():
    return jsonify(tokenLockService.delete(request.get_json()))