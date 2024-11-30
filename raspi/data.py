import serial
import re
import time

ser = serial.Serial("/dev/ttyACM0", 115200, timeout=1)

ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
time.sleep(2)

def extract_values(s):
    # Find matches for x, y, and t as floats
    x_match = re.search(r'x([-+]?\d+\.\d+)', s)
    y_match = re.search(r'y([-+]?\d+\.\d+)', s)
    t_match = re.search(r't([-+]?\d+\.\d+)', s)
    
    # Check if all matches are found
    if not (x_match and y_match and t_match):
        raise ValueError(f"Could not extract all values from string: {s}")
    
    # Extract and convert to floats
    x = float(x_match.group(1))
    y = float(y_match.group(1))
    t = float(t_match.group(1))
    
    return x, y, t

while True:
    ser.flushInput()
	# read to get the acknowledgement from the Arduino
    while True:
        line = ser.readline()
        if line:
            x, y , t = extract_values(line.decode("utf-8"))
            break

    print(f'Arduino sent: x:{x}, y:{y}, t:{t}')