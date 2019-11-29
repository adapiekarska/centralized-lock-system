from flask import jsonify, Blueprint, request

from service.access_service import AccessService
from service.auth_mapping_service import AuthMappingService

accessService = AccessService()
authMappingService = AuthMappingService()
access_resource = Blueprint('access_resource', __name__)


@access_resource.route("/api/accesses", methods=["POST"])
def create_access():
    return jsonify(accessService.create(request.get_json()))


@access_resource.route("/api/accesses", methods=["GET"])
def get_accesses():
    return jsonify(accessService.get_all())


@access_resource.route("/api/accesses/<_id>", methods=["GET"])
def get_access(_id):
    access = accessService.get_access(_id)[0]
    return jsonify(access)


@access_resource.route("/api/accesses/<_id>", methods=["DELETE"])
def delete_access(_id):
    return jsonify(accessService.delete(_id))