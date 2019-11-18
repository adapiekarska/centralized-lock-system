from model.auth_mapping import AuthMapping


class AuthMappingService:
    def __init__(self):
        self.model = AuthMapping()

    def create(self, params):
        self.model.create(params["token_id"], params["lock_id"])

    def get_all(self):
        return self.model.list_items()

    def delete(self, params):
        self.model.delete(params["id"])