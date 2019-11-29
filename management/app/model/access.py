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
            cursor = conn.cursor()
            cursor.execute(self.QUERY_CREATE, {"token_id": token_id,
                                                    "lock_id": lock_id,
                                                    "granted": granted,
                                                    "date": date})
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
                "token_id": row[1],
                "lock_id": row[2],
                "granted": row[3],
                "date": row[4]
            } for row in results]
            return rows_dicts

    def list_items_join(self, where=""):
        conn = create_connection('acs.db')
        with conn:
            query = f"SELECT {self.table_name}.ID, {self.table_name}.DATE, {self.table_name}.GRANTED, " \
                    f" {self.table_name}.LOCK_ID, {self.table_name}.TOKEN_ID, locks.NAME, tokens.TAG " \
                    f"from {self.table_name} INNER JOIN  locks ON {self.table_name}.LOCK_ID = locks.ID " \
                    f"INNER JOIN tokens ON {self.table_name}.TOKEN_ID = tokens.ID"
            if where != "":
                query = query + " where " + where
            results = conn.execute(query).fetchall()

        rows_dicts = [
            {
                "id": row[0],
                "date": row[1],
                "granted": row[2],
                "lock_name": row[5],
                "token_tag": row[6]
            }
            for row in results]
        return rows_dicts
