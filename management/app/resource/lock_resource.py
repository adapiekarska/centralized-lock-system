from flask import jsonify, request, Blueprint

from service.auth_mapping_service import AuthMappingService
from service.lock_service import LockService

lockService = LockService()
authMappingService = AuthMappingService()
lock_resource = Blueprint('lock_resource', __name__)


@lock_resource.route("/api/locks", methods=["POST"])
def create_lock():
    return jsonify(lockService.create(request.get_json()))


@lock_resource.route("/api/locks", methods=["GET"])
def get_locks():
    return jsonify(lockService.get_all())


@lock_resource.route("/api/locks/<_id>", methods=["GET"])
def get_lock(_id):
    lock = lockService.get_lock(_id)[0]
    tokens = authMappingService.get_all(f"lock_id={_id}")
    response = {
        "id": lock["id"],
        "tokens": tokens
    }
    return jsonify(response)


@lock_resource.route("/api/locks/<_id>", methods=["DELETE"])
def delete_lock(_id):
    return jsonify(lockService.delete(_id))