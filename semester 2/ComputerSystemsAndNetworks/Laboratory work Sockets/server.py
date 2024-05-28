import socket
import logging
import threading
import random

# Configuration
HOST = '0.0.0.0'
PORT = 1025 + 23
LOG_FILE = 'server_log.txt'

# Set up logging
logging.basicConfig(filename=LOG_FILE, level=logging.INFO, format='%(asctime)s - %(message)s')


def log_message(action, message):
    """Log a message with a specified action (Sent, Received, etc.)."""
    logging.info(f'{action} - {message}')


def receive_data(client_socket, length):
    """
    Receive data from the client socket until the specified length is reached.
    Returns the received data or None if the connection is lost.
    """
    data = b''
    while len(data) < length:
        packet = client_socket.recv(length - len(data))
        if not packet:
            return None
        data += packet
    return data


def handle_client(client_socket):
    """
    Handle the communication with a connected client.
    Alternately receive and send messages to measure network performance.
    """
    try:
        while True:
            # Receive and process the message header
            header = client_socket.recv(7)
            if not header:
                break

            # Extract command and data lengths from the header
            command_length = int(header[:2].decode())
            data_length = int(header[2:].decode())

            # Receive the command and data
            command = client_socket.recv(command_length).decode()
            data = receive_data(client_socket, data_length) if data_length > 0 else b''

            # Log the received command and data size
            log_message('Received', f'{command} with data size {data_length}')

            if command == 'WHO':
                # Respond to the WHO command with author information
                response = 'Author: Bohdan Kuznetsov, Variant: 23 - Network Performance Testing'
                client_socket.send(response.encode())
                log_message('Sent', response)
            elif command == 'TEST':
                # Respond to the TEST command and send a message back to the client
                response = f'TEST received with data size {data_length}'
                client_socket.send(response.encode())
                log_message('Sent', response)

                # Send a TEST message with random size back to the client
                message_size = random.randint(10, 50000)
                data = b'x' * message_size
                message = create_message('TEST', data)
                client_socket.send(message)
                log_message('Sent', f'TEST with data size {message_size}')

                # Receive confirmation from the client
                response = client_socket.recv(1024).decode()
                log_message('Received', response)
            else:
                # Respond to unrecognized commands
                response = 'ERROR: Unrecognized command'
                client_socket.send(response.encode())
                log_message('Sent', response)
    finally:
        client_socket.close()  # Close the client socket when done


def create_message(command, data=b''):
    """
    Create a message with a header that includes the command length (2 digits)
    and the data length (5 digits), followed by the command and data.
    """
    command_length = len(command)
    data_length = len(data)
    header = f'{command_length:02}{data_length:05}'
    message = header.encode() + command.encode() + data
    return message


def main():
    # Create a server socket and bind it to the specified host and port
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    log_message('Info', 'Server started')

    try:
        while True:
            # Accept incoming client connections
            client_socket, addr = server_socket.accept()
            log_message('Info', f'Accepted connection from {addr}')

            # Handle the client in a separate thread
            client_handler = threading.Thread(target=handle_client, args=(client_socket,))
            client_handler.start()
    finally:
        server_socket.close()  # Close the server socket when done


if __name__ == '__main__':
    main()
