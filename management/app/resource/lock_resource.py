from flask import jsonify, request, Blueprint

from service.lock_service import LockService

lockService = LockService()
lock_resource = Blueprint('lock_resource', __name__)


@lock_resource.route("/api/locks", methods=["POST"])
def create_lock():
    return jsonify(lockService.create(request.get_json()))


@lock_resource.route("/api/locks", methods=["GET"])
def get_locks():
    return jsonify(lockService.get_all())


@lock_resource.route("/api/locks", methods=["DELETE"])
def delete_lock():
    return jsonify(lockService.delete(request.get_json()))