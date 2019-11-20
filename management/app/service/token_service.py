from model.auth_mapping import AuthMapping
from model.token import Token


class TokenService:
    def __init__(self):
        self.tokenModel = Token()
        self.authMappingModel = AuthMapping()

    def create(self, params):
        token_id = params["id"]
        locks = params["locks"]
        self.tokenModel.create(token_id)
        for lock in locks:
            self.authMappingModel.create(token_id, lock["id"])

    def get_all(self):
        return self.tokenModel.list_items()

    def get_token(self, _id):
        return self.tokenModel.get_by_id(_id)

    def delete(self, _id):
        self.tokenModel.delete(_id)