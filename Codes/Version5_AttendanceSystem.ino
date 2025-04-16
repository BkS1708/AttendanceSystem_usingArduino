//This is the Final Code where LCD is also incorporated
#include <Adafruit_Fingerprint.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// EEPROM storage address
int eepromStart = 10; // Start writing user data from address 10

void setup() {
  lcd.begin(16, 2); 
  Serial.begin(9600);
  while (!Serial); // Wait for serial connection

  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor connected successfully!");
  } else {
    Serial.println("Fingerprint sensor connection failed.");
    while (1);
  }

  lcd.setCursor(0,0);
  lcd.print("System Starting...");
  delay(2000);
  lcd.clear();
  lcd.print("MPMC Project!");
  delay(1500);
  lcd.clear();
  
  showMenu();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "1") {
      addFingerprint();
    } else if (input == "2") {
      checkFingerprint();
    } else if (input == "3") {
      removeFingerprint();
    } else if (input == "4") {
      viewAllRecords();
    } else {
      Serial.println("Invalid option.");
    }

    delay(1000);
    showMenu();  // Show menu again
  }
}


void flushSerial() {
  while (Serial.available()) Serial.read();
} 

void showMenu() {
  Serial.println("\n=== Attendance System ===");
  Serial.println("1. Add New Fingerprint");
  Serial.println("2. Check Fingerprint");
  Serial.println("3. Remove Fingerprint");
  Serial.println("4. View All Attendance Records");
  Serial.print("Choose an option: ");
}


void addFingerprint() {
  flushSerial();  // Clear buffer before asking

  lcd.clear();
  lcd.print("Add Fingerprint!");
  

  int id = 0;
  Serial.println("Enter Serial Number (1-127): ");
  
  while (true) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      id = input.toInt();
      break;
    }
  }

  if (id < 1 || id > 127) {
    Serial.println("Invalid ID.");
    return;
  }

  Serial.println("Place finger on sensor...");
  if (getFingerprintEnroll(id)) {
    Serial.println("Fingerprint added!");

    // Store additional user data in EEPROM
    Serial.println("Enter Name: ");
    String name = waitForInput();

    Serial.println("Enter Roll Number: ");
    String roll = waitForInput();

    int addr = eepromStart + (id - 1) * 50;
    EEPROM.write(addr, name.length());
    for (int i = 0; i < name.length(); i++) EEPROM.write(addr + 1 + i, name[i]);

    EEPROM.write(addr + 25, roll.length());
    for (int i = 0; i < roll.length(); i++) EEPROM.write(addr + 26 + i, roll[i]);

    Serial.println("Details saved.");
  } else {
    Serial.println("Failed to enroll fingerprint.");
  }

  lcd.clear();
}


bool getFingerprintEnroll(uint8_t id) {
  int p = -1;
  Serial.println("Waiting for valid finger to enroll as #" + String(id));
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p != FINGERPRINT_OK) delay(100);
  }

  p = finger.image2Tz(1); if (p != FINGERPRINT_OK) return false;

  Serial.println("Remove finger");
  delay(2000);

  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
    delay(100);
  }

  Serial.println("Place same finger again...");
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    delay(100);
  }

  p = finger.image2Tz(2); if (p != FINGERPRINT_OK) return false;

  p = finger.createModel(); if (p != FINGERPRINT_OK) return false;

  p = finger.storeModel(id);
  return (p == FINGERPRINT_OK);
}

void checkFingerprint() {
  flushSerial();  // Clear any leftover input

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Finding");
  lcd.setCursor(0,1);
  lcd.print("Fingerprint!!");
  Serial.println("Place your finger on the sensor to check...");

  int result = waitForFingerprintID();  // Wait and get fingerprint

  if (result > 0) {
    Serial.println("Fingerprint matched with ID: " + String(result));

    // Fetch details from EEPROM
    int addr = eepromStart + (result - 1) * 50;
    int nameLen = EEPROM.read(addr);
    String name = "";
    for (int i = 0; i < nameLen; i++) name += char(EEPROM.read(addr + 1 + i));

    int rollLen = EEPROM.read(addr + 25);
    String roll = "";
    for (int i = 0; i < rollLen; i++) roll += char(EEPROM.read(addr + 26 + i));

    Serial.println("Name: " + name);
    Serial.println("Roll Number: " + roll);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print(roll);
    delay(2000);
    lcd.clear();
    
  } else {
    Serial.println(" No match found or timeout.");
  }
}

int waitForFingerprintID() {
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) { // 10 seconds timeout
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK) {
      delay(100);
      continue;
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) continue;

    p = finger.fingerSearch();
    if (p == FINGERPRINT_OK) {
      return finger.fingerID;
    }
  }

  return -1; // Timeout or not found
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}

void removeFingerprint() {
  lcd.clear();
  lcd.print("RemoveFingerprint");

  Serial.println("Enter Serial Number to delete: ");
  int id = 0;

  while (true) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      id = input.toInt();
      break;
    }
  }

  if (finger.deleteModel(id) == FINGERPRINT_OK) {
    Serial.println("Fingerprint ID #" + String(id) + " deleted.");

    // === Clear associated EEPROM data ===
    int addr = eepromStart + (id - 1) * 50;
    for (int i = 0; i < 50; i++) {
      EEPROM.write(addr + i, 0);
    }

    Serial.println("Associated data in EEPROM cleared.");
  } else {
    Serial.println("Error deleting fingerprint.");
  }
}



String waitForInput() {
  while (true) {
    if (Serial.available()) {
      return Serial.readStringUntil('\n');
    }
  }
}

void viewAllRecords() {
  Serial.println("\n=== Stored Attendance Records ===");

  // Prepare CSV header and send it over Serial
  String csvHeader = "ID, Name, Roll Number\n";
  Serial.print(csvHeader);

  for (int id = 1; id <= 127; id++) {
    // Check if fingerprint is present
    if (finger.loadModel(id) != FINGERPRINT_OK) continue;

    int addr = eepromStart + (id - 1) * 50;
    int nameLen = EEPROM.read(addr);

    if (nameLen > 0 && nameLen <= 24) {
      String name = "", roll = "";

      // Read name from EEPROM
      for (int i = 0; i < nameLen; i++) name += char(EEPROM.read(addr + 1 + i));

      // Read roll number from EEPROM
      int rollLen = EEPROM.read(addr + 25);
      if (rollLen > 0 && rollLen <= 24) {
        for (int i = 0; i < rollLen; i++) roll += char(EEPROM.read(addr + 26 + i));

        // Display the record in a human-readable format
        Serial.print("ID: ");
        Serial.print(id);
        Serial.print(" | Name: ");
        Serial.print(name);
        Serial.print(" | Roll Number: ");
        Serial.println(roll);

        // Send the record as a CSV line
        String csvLine = String(id) + "," + name + "," + roll + "\n";
        Serial.print(csvLine);
      }
    }
  }

  Serial.println("\nAll records have been sent to the serial monitor.");
}
