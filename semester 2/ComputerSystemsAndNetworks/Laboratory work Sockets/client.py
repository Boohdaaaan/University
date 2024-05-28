import socket
import time
import random
import logging

# Configuration
SERVER_IP = '127.0.0.1'
PORT = 1025 + 23
LOG_FILE = 'client_log.txt'

# Set up logging
logging.basicConfig(filename=LOG_FILE, level=logging.INFO, format='%(asctime)s - %(message)s')


def log_message(action, message):
    """Log a message with a specified action (Sent, Received, etc.)."""
    logging.info(f'{action} - {message}')


def create_message(command, data=b''):
    """
    Create a message with a header that includes the command length (2 digits)
    and the data length (5 digits), followed by the command and data.
    """
    command_length = len(command)
    data_length = len(data)
    header = f'{command_length:02}{data_length:05}'  # 2 digits for command length and 5 digits for data length
    message = header.encode() + command.encode() + data
    return message


def main():
    # Create a socket object and connect to the server
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_IP, PORT))

    try:
        # Send WHO command to the server
        message = create_message('WHO')
        client_socket.send(message)
        log_message('Sent', 'WHO')

        # Receive and log the response from the server
        response = client_socket.recv(1024).decode()
        log_message('Received', response)

        # Network performance testing
        num_messages = 5  # Number of message exchanges
        total_size = 0  # Total size of all messages
        min_size = float('inf')  # Initialize min size to a very large number
        max_size = 0  # Initialize max size to 0
        start_time = time.perf_counter()  # Start the timer

        for _ in range(num_messages):
            # Client sends a message with random size
            message_size = random.randint(10, 50000)
            total_size += message_size
            min_size = min(min_size, message_size)
            max_size = max(max_size, message_size)
            data = b'x' * message_size
            message = create_message('TEST', data)
            client_socket.send(message)
            log_message('Sent', f'TEST with data size {message_size}')

            # Receive and log the response from the server
            response = client_socket.recv(1024).decode()
            log_message('Received', response)

            # Client receives a message from the server
            response_header = client_socket.recv(7)
            if not response_header:
                break
            command_length = int(response_header[:2].decode())
            data_length = int(response_header[2:].decode())
            command = client_socket.recv(command_length).decode()
            data = client_socket.recv(data_length) if data_length > 0 else b''

            # Log the received command and data size
            log_message('Received', f'{command} with data size {data_length}')

            # Update total size, min and max sizes for the received message
            total_size += data_length
            min_size = min(min_size, data_length)
            max_size = max(max_size, data_length)

            # Send a response back to the server confirming receipt
            response = f'TEST received with data size {data_length}'
            client_socket.send(response.encode())
            log_message('Sent', response)

        end_time = time.perf_counter()  # End the timer
        total_time = end_time - start_time  # Calculate total time
        avg_message_size = total_size / (num_messages * 2)  # Average message size
        avg_time_per_message = total_time / (num_messages * 2)  # Average time per message

        # Log the results of the network performance testing
        log_message('Result', f'Total messages: {num_messages * 2}')
        log_message('Result', f'Total time: {total_time:.6f} seconds')
        log_message('Result', f'Average message size: {avg_message_size:.2f} bytes')
        log_message('Result', f'Average time per message: {avg_time_per_message:.6f} seconds')
        log_message('Result', f'Min message size: {min_size} bytes')
        log_message('Result', f'Max message size: {max_size} bytes')

    finally:
        client_socket.close()  # Close the socket connection


if __name__ == '__main__':
    main()
