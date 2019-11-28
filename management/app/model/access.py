from model.db_utils import table_names
from model.db_utils.db_utils import create_connection


class Access:
    def __init__(self):
        self.table_name = table_names.ACCESSES_TABLE
        self.QUERY_CREATE = f'INSERT into {self.table_name} (TOKEN_ID, LOCK_ID, GRANTED, DATE) values (:token_id, :lock_id, :granted, :date)'
        self.QUERY_DELETE = f'DELETE from {self.table_name} where id = :id'
        self.QUERY_GET = f'SELECT * from {self.table_name}'
        # self.QUERY_UPDATE = f'UPDATE {self.table_name} SET :set_query WHERE id = :id'

    def create(self, token_id, lock_id, granted, date):
        conn = create_connection('acs.db')
        with conn:
            return conn.execute(self.QUERY_CREATE, {"token_id": token_id,
                                                    "lock_id": lock_id,
                                                    "granted": granted,
                                                    "date": date})

    def delete(self, id):
        conn = create_connection('acs.db')
        with conn:
            return conn.execute(self.QUERY_DELETE, {"id": id})

    def get_by_id(self, id):
        where = f"id={id}"
        return self.list_items(where)

    # def update(self, id, update_dict):
    #     conn = create_connection('acs.db')
    #     with conn:
    #         set_query = " ".join([f'{column} = {value}'
    #                               for column, value in update_dict.items()])
    #
    #         conn.execute(self.QUERY_UPDATE, {'id': id, 'set_query': set_query})
    #         return self.get_by_id(id)

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
                "token_id": row[1],
                "lock_id": row[2],
                "granted": row[3],
                "date": row[4]
            } for row in results]
            return rows_dicts
