from model.card import Card


class CardService:
    def __init__(self):
        self.model = Card()

    def create(self, params):
        self.model.create(params["id"])

    def get_all(self):
        return self.model.list_items()

    def delete(self, params):
        self.model.delete(params["id"])