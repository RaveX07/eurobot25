from data import SerialManager
from position import Position
from time import sleep

pos = Position(0, 0)

if __name__ == '__main__':
    serial_manager = SerialManager()
    while True:
        pos = serial_manager.get_pos()
        serial_manager.send_pwm((255, 0), (255, 0))
        print("sent pwm")
        sleep(0.1)