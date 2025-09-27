from datetime import datetime
import socket
import signal
import sys

HOST = "0.0.0.0"  
PORT = 5050       

log_file = "/media/caninos/CARTAO/log.txt"

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((HOST, PORT))
server_socket.listen(5) 

print(f"Servidor TCP rodando em {HOST}:{PORT}. Pressione Ctrl+C para encerrar.")

def graceful_exit(signum, frame):
    print("\nEncerrando servidor...")
    try:
        server_socket.close()
    except Exception:
        pass
    sys.exit(0)

signal.signal(signal.SIGINT, graceful_exit)

def write_log(path='', message=''):
    linha = f"{datetime.now().isoformat()} - {message}\n"

    with open(path, "a", encoding="utf-8") as f:
        f.write(linha)

    print("Linha escrita:", linha.strip())

while True:
    try:
        conn, addr = server_socket.accept()
        print(f"\nConexão estabelecida com {addr}\n")
    
        while True:
            data = conn.recv(1024)  
            if not data:
                print(f"Cliente {addr} desconectou.")
                break

            try:
                text = data.decode("utf-8", errors="replace").strip()

            except Exception:
                text = repr(data)

            print(f"{text}")

            try:
                write_log(log_file, text)

            except PermissionError:
                print("Permissão negada. Tente executar com sudo ou verificar permissões do cartão.")
            except FileNotFoundError:
                print("Caminho não encontrado. Verifique se o cartão está montado.")
            except Exception as e:
                print(f"Erro inesperado: {e}")

        conn.close()

    except KeyboardInterrupt:
        graceful_exit(None, None)
    except Exception as e:
        print("Erro no servidor:", e)