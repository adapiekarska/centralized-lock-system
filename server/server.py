import socket
import sys
import ssl

# -----------  Config  ----------
PORT = 3333
# -------------------------------

server_cer  = 'CERTS/server-cert/server.cer'
server_key  = 'CERTS/server-cert/server.key'
ca_cer      = 'CERTS/ca.cer'

USE_TLS = True

def create_socket():
    family_addr = socket.AF_INET
    try:
        sock = socket.socket(family_addr, socket.SOCK_STREAM)
    except socket.error as msg:
        print('Error: ' + str(msg[0]) + ': ' + msg[1])
        sys.exit(1)
    return sock

def create_ssl_context():
    context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
    context.verify_mode = ssl.CERT_REQUIRED
    context.load_cert_chain(certfile=server_cer, keyfile=server_key)
    context.load_verify_locations(cafile=ca_cer)
    return context

def lookup_tag_in_database(tag, lock_id):
    import sqlite3
    from sqlite3 import Error
        
    
    conn = sqlite3.connect('../management/app/acs.db')
    
    with conn:
        lock_name_val = int.from_bytes(lock_id, byteorder='little')
        result = conn.execute(
            "SELECT * FROM auth_mappings \
            INNER JOIN tokens ON tokens.ID = auth_mappings.TOKEN_ID \
            INNER JOIN locks ON locks.ID = auth_mappings.LOCK_ID \
            WHERE tokens.TAG='{}' AND locks.NAME='{}'".format(tag.hex(), lock_name_val)
            ).fetchall()
        if len(result) == 0:
            return 0
        else:
            return 1

def save_access_in_database(tag, lock_name, result):
    import sqlite3
    from sqlite3 import Error
    from datetime import datetime

    conn = sqlite3.connect('../management/app/acs.db')
    with conn:
    	# fetch lock id
        lock_name_val = int.from_bytes(lock_name, byteorder='little')
        lock_id = conn.execute("SELECT ID FROM locks WHERE NAME='{}'".format(lock_name_val)).fetchall()
        if(len(lock_id) > 0):
        	lock_id = lock_id[0][0]
        else:
        	conn.execute("INSERT into locks (NAME) values ('{}')".format(lock_name_val))
        	lock_id = conn.execute("SELECT ID FROM locks WHERE NAME='{}'".format(lock_name_val)).fetchall()[0][0]
        	

        # fetch token id
        token_id = conn.execute("SELECT ID FROM tokens WHERE TAG='{}'".format(tag.hex())).fetchall()
        if(len(token_id) > 0):
        	token_id = token_id[0][0]
        else:
        	conn.execute("INSERT into tokens (TAG) values ('{}')".format(tag.hex()))
        	token_id = conn.execute("SELECT ID FROM tokens WHERE TAG='{}'".format(tag.hex())).fetchall()[0][0]
        	
       	print(lock_id)
       	print(token_id)
        res = "true" if result == 1 else "false"

        conn.execute("INSERT into accesses (TOKEN_ID, LOCK_ID, GRANTED, DATE) values\
         ({}, {}, '{}', '{}')".format(token_id, lock_id, res, datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
        

def handle_client(connstream):
    data = connstream.recv(1024)
    if data is None:
        print("No data received!")
        return

    recieved_tag = data[:5]
    lock_id = data[5:] 
    print('Received tag: ' + recieved_tag.hex())
    print('Received lock_id: ' + lock_id.hex())
    reply = lookup_tag_in_database(recieved_tag, lock_id)
    save_access_in_database(recieved_tag, lock_id, reply)
    print( "access granted" if reply == 1 else "access denied")
    cnt = connstream.send(bytes([reply]))    

def main():
    sock = create_socket()
    ssl_context = create_ssl_context()
    try:
        sock.bind(('', PORT))
        print('Socket binded')
        sock.listen(1)
        print('Socket listening')
        while True:
            newsocket, fromaddr = sock.accept()
            print('Connected by', fromaddr)
            newsocket.settimeout(15)
            try:
                if USE_TLS:
                    connstream = ssl_context.wrap_socket(newsocket, server_side= True)
                    handle_client(connstream)
                    connstream.shutdown(socket.SHUT_RDWR)
                    connstream.close()
                else:
                    handle_client(newsocket)
                    newsocket.shutdown(socket.SHUT_RDWR)
                    newsocket.close()

            except socket.error as msg:
                print(msg)
                continue
            print('Data exchange finished')
    
    except socket.error as msg:
        print('Error: ' + str(msg[0]) + ': ' + msg[1])
        sock.close()
        sys.exit(1)

if __name__ == "__main__":
    main()