import time
import _thread as thread
import serial

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
)

def read():
    while True:
        resp = ser.readline().decode()
        print(resp, end='')


thread.start_new_thread(read, ())

idx = 0
while True :
    msg = f'on:{idx}'
    ser.write(msg.encode())
    time.sleep(0.2)
    msg = f'off:{idx}'
    ser.write(msg.encode())
    time.sleep(0.2)
    idx = (idx + 1) % 3
