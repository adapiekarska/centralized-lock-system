import sqlite3
import model.create_tables as create_tables
from model import table_names

class Schema:

    def __init__(self):
        try:
            self.conn = sqlite3.connect('acs.db')
            self.conn.execute(create_tables[table_names.CARDS_TABLE])
            self.conn.execute(create_tables[table_names.LOCKS_TABLE])
            self.conn.execute(create_tables[table_names.CARDS_LOCKS_TABLE])
        except sqlite3.Error as err:
            print(err)


        #create_access_record = ''' INSERT INTO accesses(card_id,authorized) VALUES(?,?) '''
        # c = conn.cursor()
        # c.execute(create_cards_table)
        # c.execute(create_access_record, (0xcb6d520df9, 1))
        # c.execute(create_access_record, (0x32e1e31a2a, 0))
        # conn.commit()
        # conn.close()