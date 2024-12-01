from data import SerialManager
from position import Position
import time

pos = Position(0, 0, 0)

serial_manager = SerialManager()

while True:
    pos = serial_manager.get_pos()
    time.sleep(0.5)
    serial_manager.send_pwm((255, 0), (255, 0))
    print("sent pwm")
    time.sleep(0.5)
    