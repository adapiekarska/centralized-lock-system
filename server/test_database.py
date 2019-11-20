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
        c = conn.cursor()
        c.execute("SELECT * FROM accesses")
        print(c.fetchall())
        val = int.from_bytes(b'\xcbmR\r\xf9', byteorder='big')
        print(val)
        c.execute("SELECT authorized FROM accesses WHERE card_id={}".format(val))
        print(c.fetchall())
        conn.close()
