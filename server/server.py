import socket
import sys
import ssl

# -----------  Config  ----------
PORT = 3333
# -------------------------------

server_cer  = 'server.cer'
server_key  = 'server.key'
ca_cer      = 'ca.cer'

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

def handle_client(connstream):
    data = connstream.recv(1024)
    while data:
        data = data.decode()
        print('Received data: ' + data)
        reply = 'OK: ' + data
        conn.send(reply.encode())
        data = connstream.recv(1024)

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
            connstream = ssl_context.wrap_socket(newsocket, server_side= True)
            try:
                handle_client(connstream)
            finally:
                connstream.shutdown(socket.SHUT_RDWR)
                connstream.close()
    except socket.error as msg:
        print('Error: ' + str(msg[0]) + ': ' + msg[1])
        sock.close()
        sys.exit(1)

if __name__ == "__main__":
    main()