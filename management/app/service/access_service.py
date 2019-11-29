from model.access import Access


class AccessService:
    def __init__(self):
        self.accessModel = Access()

    def create(self, params):
        self.accessModel.create(params["token_id"], params["lock_id"], params["granted"], params["date"])

    def get_all(self):
        return self.accessModel.list_items_join()

    def get_token(self, _id):
        return self.accessModel.get_by_id(_id)

    def delete(self, _id):
        self.accessModel.delete(_id)