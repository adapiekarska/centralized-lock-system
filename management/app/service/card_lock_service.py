from model.card_lock import CardLock


class CardLockService:
    def __init__(self):
        self.model = CardLock()

    def create(self, params):
        self.model.create(params["card_id"], params["lock_id"])

    def get_all(self):
        return self.model.list_items()

    def delete(self, params):
        self.model.delete(params["id"])