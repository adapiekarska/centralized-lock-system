import socket
import sys
import ssl

# -----------  Config  ----------
PORT = 3333
# -------------------------------

server_cer  = 'CERTS/server-cert/server.cer'
server_key  = 'CERTS/server-cert/server.key'
ca_cer      = 'CERTS/ca.cer'

# permission_map = {
# 	[b'\xcbmR\r\xf9'] : b'\x01'
# }

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

def lookup_id_in_database(id):
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
            val = int.from_bytes(id, byteorder='big')
            c.execute("SELECT authorized FROM accesses WHERE card_id={}".format(val))
            result = c.fetchall()
            conn.close()
            if len(result) == 0:
                return 0
            else:
                return result[0][0]

def handle_client(connstream):
    data = connstream.recv(1024)
    if data is None:
        print("No data received!")
        return

    recieved_tag = data[:5]
    lock_id = data[5:] 
    print('Received tag: ' + recieved_tag.hex())
    print('Received lock_id: ' + lock_id.hex())
    reply = lookup_id_in_database(recieved_tag)
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