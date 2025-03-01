from model.auth_mapping import AuthMapping
from model.lock import Lock


class LockService:
    def __init__(self):
        self.lockModel = Lock()
        self.authMappingModel = AuthMapping()

    def create(self, params):
        lock_id = self.lockModel.create(params["name"])
        if 'tokens' in params:
            for token in params['tokens']:
                self.authMappingModel.create(token["id"], lock_id)

    def get_all(self):
        return self.lockModel.list_items()

    def get_lock(self, _id):
        return self.lockModel.get_by_id(_id)

    def delete(self, _id):
        self.lockModel.delete(_id)