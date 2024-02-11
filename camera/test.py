import serial
import time

def send_and_receive_data(byte_array, serial_port, baud_rate=9600):
    # Open serial port
    ser = serial.Serial(serial_port, baud_rate, timeout=1)

    try:
        # Print the byte array as integer
        sent_value = int.from_bytes(byte_array, byteorder='big')
        print(f"Sent Value (as integer): {sent_value}")

        # Send the byte array to Arduino
        ser.write(byte_array)

        # Wait for a moment to allow Arduino to process and respond
        time.sleep(0.1)

        # Read the response from Arduino
        received_byte = ser.read()
        received_value = int.from_bytes(received_byte, byteorder='big')

        # Print the received value as integer
        print(f"Received Value (as integer): {received_value}")

    finally:
        # Close the serial port
        ser.close()

if __name__ == "__main__":
    # Example byte array to be sent
    data_to_send = b'\x42'  # Replace with your byte array

    # Serial port configuration, replace 'COM3' with the actual port on your system
    serial_port = 'COM4'
    
    # Baud rate, make sure it matches the Arduino's baud rate
    baud_rate = 9600

    # Call the function to send and receive data
    send_and_receive_data(data_to_send, serial_port, baud_rate)
