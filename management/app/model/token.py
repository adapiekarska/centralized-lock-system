from model.db_utils import table_names
from model.db_utils.db_utils import create_connection


class Token:
    def __init__(self):
        self.table_name = table_names.TOKENS_TABLE
        self.QUERY_CREATE = f'INSERT into {self.table_name} (TAG) values (:tag)'
        self.QUERY_DELETE = f'DELETE from {self.table_name} where id = :id'
        self.QUERY_GET = f'SELECT * from {self.table_name}'
        # self.QUERY_UPDATE = f'UPDATE {self.table_name} SET :set_query WHERE id = :id'

    def create(self, tag):
        conn = create_connection('acs.db')
        with conn:
            cursor = conn.cursor()
            cursor.execute(self.QUERY_CREATE, {"tag": tag})
            return cursor.lastrowid

    def delete(self, id):
        conn = create_connection('acs.db')
        with conn:
            return conn.execute(self.QUERY_DELETE, {"id": id})

    def get_by_id(self, id):
        where = f"id={id}"
        return self.list_items(where)

    def list_items(self, where=""):
        conn = create_connection('acs.db')
        with conn:
            query = ""
            if where != "":
                query = self.QUERY_GET + " where " + where
            else:
                query = self.QUERY_GET
            results = conn.execute(query).fetchall()

            rows_dicts = [{
                "id": row[0],
                "tag": row[1]
            } for row in results]
            return rows_dicts
