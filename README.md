Arduino Mega GSM Module (SIM800L) - Usage Guide

This guide provides an overview of the keywords, their usage, and the success/error messages used in the Arduino Mega code for communicating with the GSM module (SIM800L).

PIN Connections:

- GSM Module RX Pin: Connect to Arduino Mega TX1 (Pin 18)
- GSM Module TX Pin: Connect to Arduino Mega RX1 (Pin 19)
- GSM Module Reset Pin: Connect to Arduino Mega Pin 40

Baud Rate:

- Baud Rate: 9600 (Both for GSM module and USB serial communication)


Keywords and Usage:

1. STAT:
   - Description: Retrieves the last saved result from initialization.
   - Usage: Send "STAT" command over the USB serial interface.
   - Response: The last saved result from initialization is printed to the serial monitor.

2. INIT_GSM:
   - Description: Performs initialization of the GSM module.
   - Usage: Send "INIT_GSM" command over the USB serial interface.
   - Response: Prints "GSM_READY" if initialization is successful. If any error occurs during initialization, it prints "GSM_ERR" followed by the error codes.

3. RST_GSM:
   - Description: Resets the GSM module and performs initialization.
   - Usage: Send "RST_GSM" command over the USB serial interface.
   - Response: Prints "GSM_READY" if reset and initialization are successful. If any error occurs, it prints "GSM_ERR" followed by the error codes.

4. RST_ARD:
   - Description: Performs a software reset of the Arduino.
   - Usage: Send "RST_ARD" command over the USB serial interface.

5. Sending SMS:
   - Description: Sends an SMS using the GSM module.
   - Usage: Send a string in the format "phonenumber, message" over the USB serial interface.
   - Response: Prints one of the following messages:
     - "SMS_SENT" if the SMS is sent successfully.
     - "SMS_FAIL" if there is an error while sending the SMS.
     - "SMS_TIMEOUT" if the SMS sending operation times out.

Success/Error Messages and Meanings:

- GSM_READY: Indicates that the GSM module is initialized and ready for use.
- GSM_ERR: Indicates an error during initialization.
  - Error Codes:
    - NO_GSM: GSM module functionality check failed.
    - NO_SIM: SIM card presence check failed.
    - WEAK_SIG: Signal strength check failed (weak signal).
    - NO_NET: Network connectivity check failed.
- SMS_SENT: Indicates that the SMS was sent successfully.
- SMS_FAIL: Indicates an error occurred while sending the SMS.
- SMS_TIMEOUT: Indicates that the SMS sending operation timed out.

Note: Make sure to connect the GSM module (SIM800L) to the appropriate pins on the Arduino Mega as specified in the code.


