
---

## 🛠️ Features

- 🔐 **Add Fingerprint** — Stores new fingerprints in EEPROM memory  
- ✅ **Verify Fingerprint** — Checks if a fingerprint matches a stored one  
- 🗑️ **Delete Fingerprint** — Remove stored data by ID  
- 📟 **LCD Feedback** — Displays status like “Access Granted” or “Fingerprint Added”  
- 📜 **Attendance Logging** — Records verified entries with timestamps  
- 📤 **Export Data** — Export attendance logs to CSV or PDF using Python  
- 🖥️ **Serial Monitor UI** — Easy-to-use terminal-based interaction

---

## 🧰 Hardware Requirements

| Component                     | Quantity |
|------------------------------|----------|
| Arduino Uno                  | 1        |
| R307S Fingerprint Sensor     | 1        |
| 16x2 LCD Display             | 1        |
| Breadboard                   | 1        |
| Jumper Wires                 | 10+      |
| USB Cable (Arduino to PC)    | 1        |
| Power Source (Optional)      | 1        |

---

## 💾 Software Requirements

- **Arduino IDE**  
  - Install the `Adafruit Fingerprint Sensor Library`
  - Install the `LiquidCrystal` library

- **Python 3.x**  
  - Required Modules:
    ```bash
    pip install pandas fpdf
    ```

- **Git & GitHub Desktop** (for version control)

---

## ⚙️ Setup Instructions

### 🔌 Arduino Setup
1. Connect the R307S sensor to Arduino (TX, RX, VCC, GND).
2. Connect the 16x2 LCD via digital pins (RS, EN, D4-D7).
3. Upload `attendance_system.ino` to your Arduino using the Arduino IDE.

### 💻 Python Setup
1. Make sure Python 3 is installed.
2. Place `export_attendance.py` in the `code/` folder.
3. Run the script to export attendance:
   ```bash
   python export_attendance.py
