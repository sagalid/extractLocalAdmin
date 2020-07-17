# extractLocalAdmin
Este proyecto permite obtener datos de un host windows como su nombre y usuarios existentes en el grupo local de administradores. Dichos datos son enviados a un servidor por socket.

# Cliente
El cliente tiene la funcionalidad de en un solo consumo, enviar la salida de los comandos configurados en en el código.
Estos comandos podrían ser extraidos desde un sitio, permitiendo con ello, comandos dinámicos.

# Servidor
El servidor con sockets a la escucha, permite ser ejecutado en un ambiente con interprete Python 3.8.
El se encargará de recibir la informació enviada por el cliente.

# Compilación del ejecutable desde linux
x86_64-w64-mingw32-gcc -o check.exe check.c -lws2_32

# Ejecución del server con Python
python3.8 sserver.py 192.168.0.100 4444 password

# Es importante destacar que la clave, IP y puerto tanto del cliente como del servidor deben coincidir.
