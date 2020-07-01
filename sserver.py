import socket
import threading
import socketserver
import datetime
import sys
import errno

global_lock = threading.Lock()
filename = "registro_clientes.txt"

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        self.obtiene_y_registra_datos()
    def obtiene_y_registra_datos(self):
        print("[!] Server invocado")
        continua = True
        buffer = ''
        while continua:
            try:
                data = str(self.request.recv(1024), 'utf-8')
                buffer += data
                if '\n' in buffer and len(buffer) > 1:
                    continua = False
            except socket.error as e:
                err = e.args[0]
                if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                    sleep(1)
                    continue
                else:
                    sys.exit(1)
        try:
            lista_registros = buffer.split(",")
            data = data.split(",")
            data = data[1:]
            tmp = ""
            for elemento in data:
                tmp += " " + elemento
            data = tmp
            if lista_registros[0] == password:
                global_lock.acquire()
                with open(filename, "a") as f:
                    fecha_hora = datetime.datetime.now().strftime('%d-%m-%Y %H:%M:%S')
                    print(f"{self.client_address[0]},{fecha_hora},{data}")
                    f.write(f"{self.client_address[0]},{fecha_hora},{data}")
                    f.close()
                global_lock.release()
                cur_thread = threading.current_thread()
                response = bytes("Datos obtenidos", 'utf-8')
                self.request.sendall(response)
            else:
                response = bytes("Nop!", 'utf-8')
                self.request.sendall(response)
        except:
            response = bytes("Error en procesamiento de datos", 'utf-8')
            self.request.sendall(response)



class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

if __name__ == "__main__":
    password=""
    if(len(sys.argv)<4):
        print("[!] Favor proveer IP, Puerto y clave.")
        print("Ejemplo:")
        print(f"python {sys.argv[0].split('/')[-1]} 127.0.0.1 443 password\n")
        exit(1)
    else:
        print("\n")
        HOST = sys.argv[1]
        PORT = int(sys.argv[2])
        try:
            password = sys.argv[3]
            print("[+] Password seteada!")
        except:
            password = ''
            print("[!] Servicio sin password!, muy peligroso y mala idea!")
    try:
        print(f"[+] Levantando el servidor: {HOST}:{PORT}...", end="")
        server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)

        try:
            ip, port = server.server_address
            server_thread = threading.Thread(target=server.serve_forever)
            server_thread.daemon = True
            server_thread.start()
            print("OK")
            print("[+] Server esperando peticiones!")
            server_thread.join()
        except Exception as err:
            print(err)
        finally:
            server.shutdown()
            server.server_close()
    except KeyboardInterrupt as ke:
        print("Bye!")
    except Exception as e:
        print(e)
        exit(1)
