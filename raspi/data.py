import serial
import re
import time
from position import Position

class SerialManager():
    def __init__(self, port="/dev/ttyACM0", baud_rate=115200):
        self.ser = serial.Serial(port, baud_rate, timeout=3)
        
        self.ser.setDTR(False)
        time.sleep(1)
        self.ser.flushInput()
        self.ser.setDTR(True)
        time.sleep(2)

    # serial string to x, y, t
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
    
    def send_pwm(self, left_pwm: tuple[int], right_pwm: tuple[int]):
        pwm_string = f"{left_pwm[0]};{left_pwm[1]};{right_pwm[0]};{right_pwm[1]}\n"
        pwm_as_bytes = str.encode(pwm_string) # convert string to bytes
        self.ser.write(pwm_as_bytes)
    
    
    def read_input(self) -> str:
        # flush input to get the latest data
        self.ser.flushInput()

        while True:
            line = self.ser.readline().decode("utf-8")
            
            if line and line[0] == 'x': # make shure to get complete data
                return line
    
    def get_pos(self) -> Position:
        serial_input = self.read_input() # get latest input
        x, y, t = self.extract_values(serial_input) # extract x y theta from serial data

        print(f'Arduino sent: x:{x}, y:{y}, t:{t}')
        return Position(x, y, t)
    