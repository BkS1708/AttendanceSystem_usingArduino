import serial
import time

# Open serial port (update COM port if needed)
ser = serial.Serial('COM4', 9600, timeout=2)
time.sleep(2)  # Let Arduino reset

print("Connected to Arduino on", ser.port)
print("Type 4 and press Enter to download attendance records\n")

while True:
    user_input = input(">> ")

    if user_input.strip() == "4":
        ser.write(b'4\n')
        print("📥 Fetching records from Arduino...\n")

        valid_csv_lines = []
        start = time.time()

        while True:
            line = ser.readline().decode().strip()
            if line:
                print(line)

                # Save only valid CSV lines (header or lines with 2 commas)
                if line.startswith("ID, Name, Roll Number") or line.count(",") == 2:
                    valid_csv_lines.append(line)

                start = time.time()  # Reset timer on activity

            if time.time() - start > 3:
                break

        if valid_csv_lines:
            with open("attendance_records.csv", "w") as file:
                for line in valid_csv_lines:
                    file.write(line + "\n")
            print(" Records saved to attendance_records.csv\n")
        else:
            print(" No valid attendance records found.\n")

    elif user_input.strip().lower() in ["exit", "quit"]:
        print("👋 Exiting script.")
        ser.close()
        break

    else:
        print("❗Invalid input. Only '4' is supported right now.")