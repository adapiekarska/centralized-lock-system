from flask import jsonify, request, Blueprint

from service.auth_mapping_service import AuthMappingService

authMappingService = AuthMappingService()
auth_mapping_resource = Blueprint('auth_mapping_resource', __name__)


@auth_mapping_resource.route("/api/auth_mappings", methods=["POST"])
def create_token():
    return jsonify(authMappingService.create(request.get_json()))


@auth_mapping_resource.route("/api/auth_mappings", methods=["GET"])
def get_tokens():
    return jsonify(authMappingService.get_all())


@auth_mapping_resource.route("/api/auth_mappings/<_id>", methods=["DELETE"])
def delete_token(_id):
    return jsonify(authMappingService.delete(_id))