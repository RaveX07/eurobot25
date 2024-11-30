import serial
import re
import time
from position import Position

class SerialManager():
    def __init__(self, port="/dev/ttyACM0", baud_rate=115200):
        self.ser = serial.Serial(port, baud_rate, timeout=1)
        
        self.ser.setDTR(False)
        time.sleep(1)
        self.ser.flushInput()
        self.ser.setDTR(True)
        time.sleep(2)

    # serial string to x, y, z
    def extract_values(self, s):
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
    
    def get_pos(self):
        self.ser.flushInput()
        
        while True:
            line = self.ser.readline()
            if line:
                x, y, t = self.extract_values(line.decode("utf-8"))
                break

        print(f'Arduino sent: x:{x}, y:{y}, t:{t}')
        return Position(x, y, t)

manager = SerialManager()
while True:
    manager.get_pos()
    time.sleep(1)