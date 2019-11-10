from model.db_utils import table_names
from model.db_utils.create_tables import create_tables
from model.db_utils.db_utils import create_connection


class Schema:

    def __init__(self):
        conn = create_connection('acs.db')
        with conn:
            conn.execute(create_tables[table_names.CARDS_TABLE])
            conn.execute(create_tables[table_names.LOCKS_TABLE])
            conn.execute(create_tables[table_names.CARDS_LOCKS_TABLE])
