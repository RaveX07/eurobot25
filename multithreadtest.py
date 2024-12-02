import threading
import time

x = 0

def add_x():
    while True:
        time.sleep(1)
        global x
        x += 1
        print(x)
    
proc = threading.Thread(target=add_x, args=[])

if __name__ == '__main__':
    proc.start()

while True: 
    print(x) 