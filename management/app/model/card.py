import sqlite3

from model import table_names


class Card:

    def __init__(self):
        self.table_name = table_names.CARDS_TABLE
        self.conn = sqlite3.connect('acs.db')

    def create(self, id):
        query = f'insert into {self.table_name} ' \
                f'(ID) ' \
                f'values ("{id}")'

        result = self.conn.execute(query)
        return result

    def get_by_id(self, id):
        where = f"AND id={id}"
        return self.list_items(where)

    def update(self, id, update_dict):
        set_query = " ".join([f'{column} = {value}'
                              for column, value in update_dict.items()])

        query = f"UPDATE {self.table_name} " \
                f"SET {set_query} " \
                f"WHERE id = {id}"
        self.conn.execute(query)
        return self.get_by_id(id)

    def list_items(self, where=""):
        query = f"SELECT ID " \
                f"from {self.table_name} " + where
        print(query)
        result_set = self.conn.execute(query).fetchall()
        result = [{column: row[i]
                   for i, column in enumerate(result_set[0].keys())}
                  for row in result_set]
        return result
