#!/usr/bin/python3
import serial
import time

# Serial port settings
SERIAL_PORT = '/dev/ttyACM0'  # Update with the correct serial port
BAUD_RATE = 9600

# Phone number and messages
PHONE_NUMBER = "+263772xxxxx"  # Update with the desired phone number
MESSAGES = [
    "Message 1",
    "Message 2",
    "Message 3",
    "Message 4",
    "Message 5",
    "Message 6",
    "Message 7",
    "Message 8",
    "Message 9",
    "Message 10"
]

# Initialize the serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
time.sleep(2)  # Wait for the connection to stabilize

def send_command(command):
    ser.write(command.encode('utf-8'))
    #time.sleep(0.05)
    response = ser.readline().decode('utf-8').strip()
    return response

def check_gsm_status():
    error_codes = {
        "NO_GSM": "GSM module functionality check failed",
        "NO_SIM": "SIM card presence check failed",
        "WEAK_SIG": "Signal strength check failed (weak signal)",
        "NO_NET": "Network connectivity check failed"
    }

    response = send_command("STAT")
    errors = response.split(", ")

    if any(error in errors for error in error_codes.keys()):
        print("Error: ", end="")
        for error in errors:
            if error in error_codes:
                error_message = error_codes[error]
                print(f"{error} ({error_message})", end=" | ")
        print()
        time.sleep(5)  # Wait 5 seconds before retrying
        response = send_command("STAT")
        errors = response.split(", ")

        if any(error in errors for error in error_codes.keys()):
            print("Retry Error: ", end="")
            for error in errors:
                if error in error_codes:
                    error_message = error_codes[error]
                    print(f"{error} ({error_message})", end=" | ")
            print()
            return False

    return True

def send_messages():
    message_queue = MESSAGES.copy()
    while message_queue:
        message = message_queue[0]
        command = f"{PHONE_NUMBER}, {message}"
        print(f"{PHONE_NUMBER}, {message}")
        response = send_command(command)
        if response == "SMS_SENT":
            message_queue.pop(0)
            print(f"Success: {response}")
            time.sleep(3)
        else:
            message_queue.append(message_queue.pop(0))
            print(f"Error: {response}")
            time.sleep(3)

# Check GSM module status
if check_gsm_status():
    send_messages()
else:
    time.sleep(5)  # Wait 5 seconds before retrying
    if check_gsm_status():
        send_messages()

# Close the serial connection
ser.close()

