from data import SerialManager
from position import Position
from time import sleep
from threading import Event, Thread
import random

pos = Position(0, 0)

pwm_left = (0, 0)
pwm_right = (0, 0)

stop_pwm_event = Event()
stop_serial_event = Event()

def serial_process():
    serial_manager = SerialManager()
    while not stop_serial_event.is_set():
        global pos
        pos = serial_manager.get_pos()
        serial_manager.send_pwm(pwm_left, pwm_right)
        print(f"sent pwm {pwm_left[0]}, {pwm_left[1]}, {pwm_right[0]}, {pwm_right[1]}")
        sleep(0.05)
        
def pwm_process():
    while not stop_pwm_event.is_set():
        global pwm_left, pwm_right
        pwm_right = (random.randint(0,255), random.randint(0,255))
        pwm_left = (random.randint(0,255), random.randint(0,255))
        sleep(1)
        
serial_thread= Thread(target=serial_process, args=())
pwm_thread= Thread(target=pwm_process, args=())

if __name__ == '__main__':
    try:
        serial_thread.start()
        pwm_thread.start()
        
        serial_thread.join()
        pwm_thread.join()
        
    except KeyboardInterrupt:
        print("Stopping threads")
        
    finally:
        stop_serial_event.set()
        stop_pwm_event.set()
        
        # Wait for threads to finish
        if serial_thread:
            serial_thread.join()
        if pwm_thread:
            pwm_thread.join()
        
        print("All threads stopped")
    
