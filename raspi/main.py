from data import SerialManager
from position import Position
from time import sleep
from threading import Thread

pos = Position(0, 0)

pwm_left = (0, 0)
pwm_right = (0, 0)

def serial_process():
    serial_manager = SerialManager()
    while True:
        global pos
        pos = serial_manager.get_pos()
        serial_manager.send_pwm(pwm_left, pwm_right)
        print(f"sent pwm {pwm_left[0]}, {pwm_left[1]}, {pwm_right[0]}, {pwm_right[1]}")
        sleep(0.1)
        
serial_thread= Thread(target=serial_process, args=())

if __name__ == '__main__':
    serial_thread.start()
    
while True:
    pwm_left[0:1] += 1
    pwm_right[0:1] += 1