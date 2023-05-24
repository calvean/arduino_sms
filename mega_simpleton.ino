//#include <Arduino.h>

//#define GSM_RX_PIN 19  // RX pin of GSM module (connected to Arduino TX1)
//#define GSM_TX_PIN 18  // TX pin of GSM module (connected to Arduino RX1)
#define GSM_RST_PIN 40 // GSM module reset pin

#define TIMEOUT_MS 10000 // Timeout for GSM operations in milliseconds

String stat = ""; // Global variable to store GSM module status

void initializeGSM() {
  Serial1.begin(9600); // Initialize Serial1 communication for GSM module
  Serial.begin(9600);  // Initialize Serial communication via USB
  stat = "";
  delay(100);
  // Check GSM module functionality
  Serial1.println("AT");
  delay(100);
  if (Serial1.find("OK")) {
    stat += "GSM_OK,";
  } else {
    stat += "NO_GSM,";
  }

  // Check SIM presence
  Serial1.println("AT+CPIN?");
  delay(100);
  if (Serial1.find("READY")) {
    stat += "SIM_OK,";
  } else {
    stat += "NO_SIM,";
  }

  // Check signal strength
  Serial1.println("AT+CSQ");
  delay(100);
  if (Serial1.find("+CSQ")) {
    int index = Serial1.parseInt();
    if (index >= 10) {
      stat += "SIG_OK,";
    } else {
      stat += "WEAK_SIG,";
    }
  }

  // Check network connectivity
  Serial1.println("AT+CREG?");
  delay(100);
  if (Serial1.find("+CREG: 0,1")) {
    stat += "NET_OK";
  } else {
    stat += "NO_NET";
  }

  if (stat.endsWith(",")) {
    stat = stat.substring(0, stat.length() - 1); // Remove trailing comma
  }

  if (stat.indexOf(',') == -1) {
    Serial.println("GSM_READY");
  } else {
    Serial.print("GSM_ERR: ");
    Serial.println(stat);
  }
}
void resetGSM() {
  pinMode(GSM_RST_PIN, OUTPUT); // Set GSM reset pin as OUTPUT
  digitalWrite(GSM_RST_PIN, HIGH); // Set GSM reset pin HIGH initially

  delay(100);
  digitalWrite(GSM_RST_PIN, LOW); // Reset the GSM module
  delay(1000);
  digitalWrite(GSM_RST_PIN, HIGH); // Set GSM reset pin HIGH

  delay(5000); // Wait for GSM module to initialize
}

void sendSMS(const String& phoneNumber, const String& message) {
  Serial1.println("AT+CMGF=1"); // Set SMS mode to text
  delay(100);
  
  Serial1.print("AT+CMGS=\"");
  Serial1.print(phoneNumber);
  Serial1.println("\"");
  delay(100);
  
  Serial1.print(message);
  Serial1.write(26); // Send Ctrl+Z to indicate end of message
  delay(TIMEOUT_MS);
  
  if (Serial1.find("OK")) {
    Serial.println("SMS_SENT");
  } else if (Serial1.find("ERROR")) {
    Serial.println("SMS_FAIL");
  } else {
    Serial.println("SMS_TIMEOUT");
  }
}

void resetArduino() {
  asm volatile ("  jmp 0");
}

void setup() {
  initializeGSM();
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "STAT") {
  //    Serial.println(stat);
      initializeGSM();
    } else if (command == "INIT_GSM") {
      resetGSM();
      initializeGSM();
    } else if (command == "RST_GSM") {
      resetGSM();
      Serial.println("GSM_RESET");
 //     initializeGSM();
    } else if (command == "RST_ARD") {
      resetArduino();
    } else {
      int commaIndex = command.indexOf(',');
      if (commaIndex != -1 && commaIndex < command.length() - 1) {
        String phoneNumber = command.substring(0, commaIndex);
        String message = command.substring(commaIndex + 1);
        sendSMS(phoneNumber, message);
      }
    }
  }
}

