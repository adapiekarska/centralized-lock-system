import sqlite3
from sqlite3 import Error
 
    
conn = None
try:
    conn = sqlite3.connect('./accesses.db')
    print(sqlite3.version)
except Error as e:
    print(e)
finally:
    if conn:

        create_access_table =   """CREATE TABLE IF NOT EXISTS accesses (
                                    card_id integer PRIMARY KEY,
                                    authorized integer NOT NULL
                                ); """
        create_access_record = ''' INSERT INTO accesses(card_id,authorized) VALUES(?,?) '''
        c = conn.cursor()
        c.execute(create_access_table)
        c.execute(create_access_record, (0xcb6d520df9, 1))
        c.execute(create_access_record, (0x32e1e31a2a, 0))
        conn.commit()
        conn.close()
