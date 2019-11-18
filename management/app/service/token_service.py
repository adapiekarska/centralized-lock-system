from model.token import Token


class TokenService:
    def __init__(self):
        self.model = Token()

    def create(self, params):
        self.model.create(params["id"])

    def get_all(self):
        return self.model.list_items()

    def delete(self, params):
        self.model.delete(params["id"])